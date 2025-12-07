

// Balduino Jose da Silva


// ============================================================================
//  ESP32 DevKit v1 + LCD 16x2 I2C (endereço 0x27) + Sensor BMP180 (Adafruit)

//  Objetivo: Mostrar temperatura (°C) e pressão atmosférica (hPa) no LCD.
// ============================================================================

// Bibliotecas necessárias
#include <Wire.h>                // Protocolo I2C
#include <LiquidCrystal_I2C.h>   // Controle do display LCD I2C
#include <Adafruit_BMP085.h>     // Biblioteca para o sensor BMP180/BMP085

// ---------------------- DEFINIÇÃO DE PINOS E LCD ----------------------------
#define SDA_PIN   21    // Pino SDA do ESP32
#define SCL_PIN   22    // Pino SCL do ESP32
#define LCD_ADDR  0x27  // Endereço I2C padrão do LCD 16x2
#define LCD_COLS  16    // Número de colunas do LCD
#define LCD_ROWS  2     // Número de linhas do LCD

// Criação dos objetos do LCD e do sensor BMP180
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
Adafruit_BMP085 bmp;   // Objeto do sensor BMP180

// ---------------------- FUNÇÃO PARA ESCREVER NO LCD -------------------------
/*
 * Função auxiliar para exibir duas linhas no LCD rapidamente.
 * l1 = texto da linha superior
 * l2 = texto da linha inferior (opcional)
 */
void lcdMsg(const char* l1, const char* l2 = "") {
  lcd.clear();                 // Limpa o display
  lcd.setCursor(0, 0); lcd.print(l1);  // Linha 1
  lcd.setCursor(0, 1); lcd.print(l2);  // Linha 2
}

// ------------------------------- SETUP --------------------------------------

void setup() {
  Serial.begin(115200);   // Inicializa monitor serial
  delay(200);

  Wire.begin(SDA_PIN, SCL_PIN);   // Inicia barramento I2C nos pinos definidos


  lcd.init();       // Inicializa o LCD
  lcd.backlight();  // Liga o backlight
  lcdMsg("ESP32 + LCD +", "   BMP180   ");  // Mensagem inicial

  // Tenta inicializar o sensor BMP180
  // Usa endereço padrão 0x77 

  if (!bmp.begin()) {

    // Se falhar, mostra aviso no LCD

    lcdMsg("BMP180 NAO ACHO", "Cheque fios/VCC");
    Serial.println("ERRO: BMP180 nao encontrado (addr 0x77).");

    // Não trava o código — tentará reconfigurar no loop()

  } else {
    lcdMsg("BMP180 OK", "Addr: 0x77");
    Serial.println("BMP180 encontrado em 0x77.");
  }

  delay(1200);
  lcd.clear();  // Limpa para iniciar exibição normal
}

// ------------------------------- LOOP ---------------------------------------

void loop() {
  static unsigned long last = 0;
  
  // Atualiza leituras a cada 2000 ms (2 segundos)

  if (millis() - last < 2000) return;
  last = millis();

  // Controle de reinicialização do BMP180 caso não inicie no começo

  static bool ok = false;

  // Se ainda não iniciou corretamente, tenta novamente

  if (!ok) {
    ok = bmp.begin();
    if (!ok) {
      lcdMsg("Sem BMP180", "Cheque I2C");
      Serial.println("Tentando novamente BMP180...");
      return; // Tenta novamente no próximo ciclo
    } else {
      lcdMsg("BMP180 OK", "Addr: 0x77");
      delay(800);
      lcd.clear();
    }
  }

  // ---------------------- LEITURA DOS DADOS DO SENSOR ----------------------

  float T = bmp.readTemperature();      // Lê temperatura em graus Celsius
  int32_t P_pa = bmp.readPressure();    // Lê pressão em Pascal (Pa)
  float P_hPa = P_pa / 100.0f;          // Conversão para hPa (hectopascal)

  // ---------------------- EXIBE OS DADOS NO LCD ----------------------------
  
  lcd.setCursor(0, 0);
  lcd.print("T:"); 
  lcd.print(T, 1);                      // 1 casa decimal
  lcd.print((char)223);                 // Caractere do símbolo ° no LCD
  lcd.print("C     ");                  // Espaços extras evitam "lixo" no LCD

  lcd.setCursor(0, 1);
  lcd.print("P:"); 
  lcd.print(P_hPa, 1);                  // 1 casa decimal
  lcd.print(" hPa   ");                 // Espaços extras para limpeza

  // ---------------------- LOG NO MONITOR SERIAL ----------------------------
  
  Serial.printf("T=%.2f C  P=%.2f hPa\n", T, P_hPa);
}


