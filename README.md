# Projeto-Integrador-1-UFSC-2025.2-

Sistema Inteligente para Automação e Monitoramento de Condições Ambientais em Residências

# DESCRIÇÂO

O projeto propõe um Sistema Inteligente para Automação e Monitoramento de Condições Ambientais em Residências, capaz de coletar, exibir e reagir a variáveis de ambiente em tempo real. O núcleo é um ESP32 conectado a sensores de temperatura/umidade (DHT22), pressão atmosférica (BMP180 ou equivalente) e gás inflamável/fumaça (MQ-2). As leituras são apresentadas localmente em um display LCD/OLED e remotamente no aplicativo ESP RainMaker, permitindo acompanhamento contínuo e registro simplificado.

Além do monitoramento, o sistema executa automação doméstica: controla ao menos um atuador — uma lâmpada via relé de estado sólido (SSR) — que pode ser acionada manualmente (um clique no app-RAINMAKER) ou de forma automática conforme parâmetros definidos pelo usuário (por exemplo, ligar a lâmpada quando a luminosidade/estado desejado exigir ou como alerta visual em caso de gás). Para interação natural, o sistema responde a comandos de voz via Alexa, possibilitando ligar/desligar a lâmpada e consultar o estado dos sensores por voz. A arquitetura prioriza simplicidade de instalação, operação confiável (histerese e “report-on-change”), e extensibilidade para novos sensores/atuadores.
# Monitor Ambiental ESP32 + RainMaker

# OBJETIVO DO PROJETO


# Objetivo Geral

Desenvolver um protótipo funcional de monitoramento ambiental integrado à automação residencial, com visualização local e em nuvem, e controle por voz (Alexa), que permita medir, notificar e atuar sobre o ambiente doméstico de forma segura, responsiva e configurável pelo usuário.

# Objetivos Específicos

1. Aquisição de dados ambientais

      -Implementar a leitura contínua de temperatura/umidade (DHT22), pressão (BMP180/BMP280) e gás (MQ-2), com debounce lógico e histerese para estados OK / ATENÇÃO / ALERTA.

      -Calibrar faixas e limiares de alerta do MQ-2 para reduzir falsos positivos.

2. Visualização e telemetria

      -Exibir, no LCD/OLED, os valores com unidades (°C, %, hPa) e o estado do gás.

3. Automação de atuadores

   -Controlar uma lâmpada via SSR (GPIO dedicado) com:

      -Ação manual (botão 1-clique no app).

      -Ação automática baseada em parâmetros definidos pelo usuário (ex.: uso como alerta visual quando ALERTA no MQ-2).

   -Registrar e sincronizar estado do atuador entre dispositivo, app e comandos de voz.

4. Integração com Alexa

      -Disponibilizar rotinas/comandos para ligar/desligar a lâmpada e consultar leituras básicas (temperatura/umidade/pressão).

      -Assegurar autenticação e fluxo de provisionamento simples (ex.: SoftAP/POP) para onboarding do usuário.

---

## Funcionalidades

- Leitura de **Temperatura (°C)** e **Umidade (%)** via **DHT22**.
- Leitura de **Pressão (hPa)** via **BMP180 (GY‑68)**.
- Leitura de **nível (%)** do **MQ‑2** + classificação de **Estado**: `OK`, `ATENÇÃO`, `ALERTA`.
- Exibição local no **LCD 16x2 I²C** (0x27).
- Cartões no **ESP RainMaker**:
  - **Temperatura**, **Umidade**, **Pressão**.
  - **MQ2**: `Nivel(%)`, `Estado`, `AlarmActive` (somente leitura).
  - **Alarme** (Power) – **um clique**. Mostra também `Estado` e `AlarmActive` nos detalhes.
  - **Lâmpada** (Power) – **um clique**.
- Botão físico (compartilhado no pino do LED de alerta) para alternar o **Power** do **Alarme**.

- Em resumo neste sistema embarcado usou-se como Microcontrolador o **ESP-WROOM-32D** para, captar os dados de sensores como de **Temperatura, Umidade, Pressão** e **gás (MQ‑2)**, com **LCD 16x2 I²C** e controle remoto via **ESP RainMaker**. Possui dois atuadores com **um clique(Liga/Desliga)** no app(Rain Maker):
- **Alarme** (GPIO33): liga automaticamente quando o estado do MQ‑2 entra em **ALERTA**, desde que o usuário tenha deixado o *Power* ligado.
- **Lâmpada** (GPIO23):  e controle por voz no app da amazon alexa.
Provisionamento por **SoftAP/BLE** (POP/senha: `abcd1234`) e particionamento(Arduino-IDE) **RainMaker 4MB No OTA** (recomendado), para não dar problema de espaço no ato de carregar o codigo no microcontrolador.

---

## Hardware
- **ESP32** (4 MB Flash).
- **DHT22** (GPIO4).
- **BMP180 (GY‑68)** I²C em 0x77 (SDA=GPIO21, SCL=GPIO22).
- **LCD 16x2 I²C** 0x27 (SDA=GPIO21, SCL=GPIO22).
- **MQ‑2** (AO → **GPIO34**, Vcc 5V, GND) com divisor do módulo.
- **Alarme** (relé/buzzer) no **GPIO33**.
- **SSR 1 canal** da **Lâmpada** no **GPIO23**.
- Fonte 5V/3V3 estável e aterramento comum.

> Observação: a biblioteca `LiquidCrystal_I2C` pode emitir um *warning* genérico de compatibilidade – **pode ser ignorado** para ESP32 na prática.

### Ligações resumidas
```
I²C:   SDA -> 21, SCL -> 22  (BMP180 0x77, LCD 0x27)
DHT22: DATA -> 4
MQ-2:  AO   -> 34   (usar GND comum; Vcc do módulo e divisor já presentes)
Alarme: GPIO33 -> driver/relé/buzzer
Lâmpada(SSR): GPIO23 -> SSR (AC isolado)
```

---

## Software (visão geral)
- **Arduino IDE** com **Espressif ESP32 core**.
- Bibliotecas:
  - `ESP RainMaker` / `RMaker` e `WiFiProv`
  - `Adafruit BMP085` (BMP180)
  - `DHT sensor library`
  - `LiquidCrystal_I2C`
- **Provisionamento**: **SoftAP** (`PROV_xxxxxx`, senha/POP `abcd1234`).

Consulte **[INSTALL.md](INSTALL.md)** para o passo‑a‑passo completo de setup e **[docs/arquitetura.md](docs/arquitetura.md)** para a arquitetura do sistema.

---

## Uso
1. Energize o ESP32 com o hardware conectado.
2. Abra o app **ESP RainMaker** e escolha **Provision via SoftAP**.
3. Conecte-se ao SSID exibido (`PROV_xxxxxx`) com a senha `abcd1234`.
4. Siga o assistente do app para configurar Wi‑Fi e vincular o dispositivo.
5. Utilize os **cartões** para visualizar variáveis e controlar **Alarme**/**Lâmpada**.

---

---

## Imagens

<img width="793" height="322" alt="image" src="https://github.com/user-attachments/assets/b4527c04-1a58-491a-b196-427f774ed8d3" />

<img width="881" height="382" alt="image" src="https://github.com/user-attachments/assets/8542438a-301b-451d-ba62-4a66cb154ed3" />
<img width="884" height="373" alt="image" src="https://github.com/user-attachments/assets/488fc413-b776-4998-8f24-c72cfeb13f03" />
<img width="901" height="387" alt="image" src="https://github.com/user-attachments/assets/a7d6f4a4-ba15-46af-85dd-107a8dd38629" />
<img width="657" height="346" alt="image" src="https://github.com/user-attachments/assets/8e404eb9-8ccc-48f2-8a88-c153abaa2849" />
<img width="421" height="425" alt="image" src="https://github.com/user-attachments/assets/c71706ae-64bf-4fa4-9095-babe8659861b" />
<img width="588" height="296" alt="image" src="https://github.com/user-attachments/assets/34074709-81b3-4acd-b9a0-126c37476c75" />










