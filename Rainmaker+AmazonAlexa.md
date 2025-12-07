

#  ESP32 – Monitoramento Ambiental + ESP RainMaker + Amazon Alexa  
### DHT22 • BMP180 • MQ-2 • LCD 16x2 • LEDs • Relé • Automação Inteligente

Este projeto implementa um sistema completo de **monitoramento e automação ambiental** utilizando o **ESP32**, integrado ao **ESP RainMaker** e compatível com **Amazon Alexa**, permitindo tanto o controle remoto quanto consultas por voz.

---

##  Funcionalidades Principais

###  Sensores
- **Temperatura e Umidade (DHT22)**  
- **Pressão Atmosférica (BMP180)**
- **Detecção de Gases Inflamáveis (MQ-2)**  
  - Três estados com histerese: **OK**, **ATENÇÃO**, **ALERTA**

###  Ações e Automação
- **Relé / Alarme (GPIO33)**  
  - Pode ligar automaticamente em ALERTA  
  - Pode ligar/desligar manualmente pelo RainMaker ou Alexa  
- **Lâmpada / SSR (GPIO23)** – controlada via app ou voz  
- **Botão local** para habilitar/desabilitar o alarme automático  
- **LEDs indicadores de estado** (verde, amarelo, vermelho)

###  Interface Local
- **LCD 16x2 I2C** alternando entre:  
  - Tela ambiente (T, U, Pressão)  
  - Tela MQ-2 (Δ%, tensão e estado)  

###  Nuvem e Voz
- Monitoramento remoto via **ESP RainMaker**
- Controle via **Amazon Alexa**
- Dados acessíveis por voz:
  - “Alexa, qual a umidade do *Umidade Alexa*?”
  - “Alexa, qual a pressão do *Pressao Alexa*?”
  - “Alexa, qual o nível do *MQ2 Alexa*?”

---

# 🗂 Estrutura dos Dispositivos no RainMaker

| Dispositivo RainMaker | Tipo | Função |
|-----------------------|------|--------|
| Temperatura           | Sensor | Mostra temperatura em texto |
| Umidade               | Sensor | Umidade (%) |
| Pressao               | Sensor | Pressão (hPa) |
| MQ2                   | Switch | Habilitar/Desabilitar alarme automático + Estado |
| Alarme                | Lightbulb | Relé/Buzzer (manual) |
| Lampada               | Lightbulb | Acende/Apaga lâmpada |
| **Umidade Alexa**     | Temp Sensor | Espelho para Alexa |
| **Pressao Alexa**     | Temp Sensor | Espelho para Alexa |
| **MQ2 Alexa**         | Temp Sensor | Espelho (delta %) |

Os “espelhos” são **necessários para Alexa**, pois a Alexa só interpreta sensores do tipo *temperature-sensor*.

---

#  Conexões do Hardware

| Componente | Pino ESP32 |
|------------|------------|
| DHT22      | GPIO 4     |
| MQ-2 (AO)  | GPIO 34 (ADC) |
| LED Verde (OK) | GPIO 13 |
| LED Amarelo (Atenção) | GPIO 26 |
| LED Vermelho (Alerta) | GPIO 27 |
| Relé / Alarme | GPIO 33 |
| Lampada / SSR | GPIO 23 |
| LCD I2C SDA | GPIO 21 |
| LCD I2C SCL | GPIO 22 |

---

# 📲 Passo a Passo – Configuração no ESP RainMaker

## 1️⃣ Instale o **ESP RainMaker** no celular  
Links:  
- Android: https://play.google.com/store/apps/details?id=com.espressif.rainmaker  
- iOS: https://apps.apple.com/app/id1501682788  

## 2️⃣ Ligue o ESP32  
O firmware cria automaticamente um serviço BLE.

