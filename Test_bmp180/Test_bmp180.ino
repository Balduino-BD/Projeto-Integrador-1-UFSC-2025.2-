/*
        Balduino José Da Silva
*/


// ============================================================================
//  ESP32 DevKit v1 + BMP180 (Adafruit_BMP085)
//  Exemplo simples: leitura de temperatura (°C) e pressão (hPa) via Serial.
// ============================================================================

#include <Wire.h>             // Biblioteca para comunicação I2C
#include <Adafruit_BMP085.h>  // Biblioteca do sensor BMP180/BMP085

// Pinos I2C do ESP32 (padrão)

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BMP085 bmp;          // Cria objeto do sensor

void setup() {
  Serial.begin(115200);       // Inicia comunicação serial
  delay(200);

  // Inicia I2C nos pinos definidos para o ESP32

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Teste BMP180 com ESP32...");
  Serial.println("Inicializando sensor...");

  // Tenta iniciar o sensor BMP180 (endereço padrão 0x77)
  if (!bmp.begin()) {
    Serial.println("ERRO: BMP180 nao encontrado!");
    Serial.println("Verifique ligacoes: VCC, GND, SDA, SCL e endereco (0x77).");

    // Se quiser travar aqui, pode usar while(1);
    // while (1);

  } else {
    Serial.println("BMP180 encontrado com sucesso!");
  }
}

void loop() {

  // Se o sensor não foi inicializado com sucesso, não adianta ler

  static bool ok = false;
  if (!ok) {
    ok = bmp.begin();
    if (!ok) {
      Serial.println("Ainda sem BMP180... tentando novamente.");
      delay(2000);
      return;
    } else {
      Serial.println("BMP180 OK! (novo sucesso de init no loop)");
    }
  }

  // Leitura da temperatura em °C

  float temperatura = bmp.readTemperature();

  // Leitura da pressão em Pascal (Pa)

  int32_t pressaoPa = bmp.readPressure();

  // Converte para hPa (hectopascal) -> 1 hPa = 100 Pa

  float pressao_hPa = pressaoPa / 100.0f;

  // (Opcional) Cálculo de altitude aproximada (nível do mar padrão: 1013.25 hPa)

  float altitude = bmp.readAltitude(1013.25);

  // Exibe valores no Serial Monitor
  
  Serial.println("====================================");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Pressao:     ");
  Serial.print(pressao_hPa);
  Serial.println(" hPa");

  Serial.print("Altitude aprox.: ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println("====================================\n");

  delay(2000);  // Aguarda 2 segundos antes da proxima leitura
}
