#include <Arduino.h>
#include <nvs_flash.h>
#include <Preferences.h>

// ==== pinos e constantes iguais ao teu projeto ====
#define MQ2_ADC_PIN   34
#define MQ2_SAMPLES   32
#define ADC_MAX       4095.0f
#define ADC_REF_V     3.3f
#define AO_DIV_FACTOR 2.0f   // teu divisor A0 para 3V3 do ESP (x2)
#define MQ2_VC_MOD_V  5.0f
#define MQ2_RL_KOHM   5.0f
#define RS_R0_CLEAN_AIR 9.8f

Preferences prefs;

static uint16_t readADCavg(int pin, int samples){
  uint32_t acc = 0;
  for (int i = 0; i < samples; i++){ acc += analogRead(pin); delayMicroseconds(250); }
  return (uint16_t)(acc / samples);
}
static float adcToAO_V(uint16_t raw){ return ((raw / ADC_MAX) * ADC_REF_V) * AO_DIV_FACTOR; }
static float calcRs_kohm(float vout){
  vout = constrain(vout, 0.01f, MQ2_VC_MOD_V - 0.01f);
  return MQ2_RL_KOHM * (MQ2_VC_MOD_V - vout) / vout;
}
static bool nvs_init_ok(){
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
    nvs_flash_erase(); err = nvs_flash_init();
  }
  return (err == ESP_OK);
}

void setup(){
  Serial.begin(115200);
  delay(400);
  Serial.println("\n=== Calibracao MQ-2 (R0) ===");
  if (!nvs_init_ok()){ Serial.println("NVS FAIL"); while (true) delay(1000); }

  analogReadResolution(12);
  analogSetPinAttenuation(MQ2_ADC_PIN, ADC_11db);

  Serial.println(">> Certifique-se de estar em AR LIMPO.");
  Serial.println(">> Aguardando estabilizacao (~6 minutos)...");
  const uint32_t T_MS = 6UL * 60UL * 1000UL;   // 6 minutos
  const uint32_t PRINT_EVERY = 10000UL;        // log a cada 10 s
  uint32_t t0 = millis(), t_last = t0;

  double accRs = 0.0; uint32_t n = 0;
  while (millis() - t0 < T_MS){
    uint16_t raw = readADCavg(MQ2_ADC_PIN, MQ2_SAMPLES);
    float vAO = adcToAO_V(raw);
    float Rs  = calcRs_kohm(vAO);
    accRs += Rs; n++;

    if (millis() - t_last >= PRINT_EVERY){
      t_last = millis();
      float pct = 100.0f * (millis() - t0) / (float)T_MS;
      Serial.printf("... %5.1f%%  vAO=%.2f V  Rs=%.3f kOhm\n", pct, vAO, Rs);
    }
    delay(200);
  }

  float Rs_avg = (n > 0) ? (float)(accRs / n) : NAN;
  float R0     = (isnan(Rs_avg) ? NAN : (Rs_avg / RS_R0_CLEAN_AIR));

  if (isnan(R0) || R0 <= 0){
    Serial.println("ERRO: R0 invalido.");
  } else {
    Serial.printf("\n>> Rs_medio = %.3f kOhm\n", Rs_avg);
    Serial.printf(">> R0 (ar limpo) = %.3f kOhm\n", R0);

    prefs.begin("mq2", false);
    prefs.putFloat("R0", R0);
    prefs.end();
    Serial.println(">> Gravado no NVS (namespace 'mq2', chave 'R0').");
  }

  Serial.println("\nFIM. Reinicie e volte para o sketch principal.");
}

void loop(){}
