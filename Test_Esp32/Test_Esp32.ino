/*

      Balduino Jose Da Silva

*/

// ============================================================================
//  TESTE BÁSICO ESP32 DEVKIT V1
//  - Pisca o LED interno da placa (LED_BUILTIN)
//  - Envia mensagens pelo Monitor Serial
// ============================================================================

void setup() {
  // Inicia a comunicação serial em 115200 bps
  Serial.begin(115200);
  delay(500);  // Pequena pausa para estabilizar

  // Configura o pino do LED interno como saída


  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Teste ESP32 iniciado!");
  Serial.println("LED interno deve piscar a cada 1 segundo...");
}

void loop() {

  // Liga o LED

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
  delay(1000);  // espera 1 segundo

  // Desliga o LED

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");
  delay(1000);  // espera 1 segundo
}
