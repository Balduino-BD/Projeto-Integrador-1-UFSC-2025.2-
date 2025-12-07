

// Balduino Da Silva

// ============================================================================
//  ESP32 WROOM DevKit v1 + LCD 16x2 I2C (endereço 0x27)
//  Objetivo: testar comunicação I2C com o LCD e exibir um contador de segundos.
// ============================================================================

#include <Wire.h>                // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h>   // Biblioteca para controle do LCD via I2C

// ---------------------- DEFINIÇÃO DE PINOS E PARÂMETROS DO LCD --------------
#define SDA_PIN 21               // Pino SDA do ESP32
#define SCL_PIN 22               // Pino SCL do ESP32
#define LCD_ADDR 0x27            // Endereço I2C do módulo LCD (muito comum)
#define LCD_COLS 16              // Número de colunas do LCD (16 caracteres)
#define LCD_ROWS 2               // Número de linhas do LCD (2 linhas)

// Cria o objeto "lcd" associado ao endereço e tamanho do display

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// ------------------------------ SETUP ---------------------------------------
void setup() {
  Serial.begin(115200);      // Inicializa comunicação serial para debug
  delay(200);                // Pequeno atraso para estabilizar

  // Inicia o barramento I2C nos pinos definidos (SDA, SCL)

  Wire.begin(SDA_PIN, SCL_PIN);

  // Inicialização do LCD:
  
  lcd.init();            



  lcd.backlight();           // Liga a luz de fundo do LCD (backlight)

  lcd.clear();               // Limpa qualquer coisa que esteja na tela
  lcd.setCursor(0, 0);       // Posiciona o cursor na coluna 0, linha 0
  lcd.print("Projeto_UFSC"); // Escreve o texto da primeira linha

  lcd.setCursor(0, 1);       // Posiciona o cursor na coluna 0, linha 1
  lcd.print("Endereco: 0x27"); // Mostra o endereço I2C do LCD
}

// ------------------------------ LOOP ----------------------------------------
void loop() {

  // Contador simples só para ver a atualização no LCD

  static unsigned long t0 = 0; // Armazena o último instante de atualização (ms)
  static unsigned long s = 0;  // Contador de segundos

  // Verifica se já passou 1 segundo (1000 ms) desde a última atualização

  if (millis() - t0 >= 1000) {
    t0 = millis();  // Atualiza o "tempo anterior" para o tempo atual
    s++;            // Incrementa o contador de segundos

    // Atualiza a linha 2 do LCD com o valor do contador

    lcd.setCursor(0, 1);   // Vai para o início da segunda linha
    lcd.print("Segundos: "); // Texto fixo
    lcd.print(s);           // Valor do contador
    lcd.print("   ");       // Espaços extras para "apagar" lixo visual se o número diminuir de dígitos
  }
}

