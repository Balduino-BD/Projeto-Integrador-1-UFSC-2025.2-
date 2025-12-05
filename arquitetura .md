[Apresentação App_RainMaker]
<img width="398" height="749" alt="image" src="https://github.com/user-attachments/assets/eccf6262-9d21-40b2-9a1e-2320d2d95dd8" />
<img width="394" height="785" alt="image" src="https://github.com/user-attachments/assets/16c90276-a26b-41c4-bf26-f8adf336827a" />



[Diagrama de Ligações]<img width="1092" height="750" alt="image" src="https://github.com/user-attachments/assets/d78f6256-3b43-4bd0-b6b3-3acbe60ed424" />

<img width="1556" height="828" alt="image" src="https://github.com/user-attachments/assets/a6017830-9706-4a5e-bfdf-8db40c2c92ce" />
<img width="1497" height="820" alt="image" src="https://github.com/user-attachments/assets/944d8082-c04b-4e96-ad68-dab864a36cb5" />
<img width="1168" height="779" alt="image" src="https://github.com/user-attachments/assets/8b7f0c9e-684a-4a6e-93a1-fa507eedd973" />




# Arquitetura do Sistema

## Visão geral
O sistema integra **sensoriamento ambiental**, **exibição local** e **controle remoto** via **ESP RainMaker**, com lógica de alarme baseada no **MQ‑2**.

```
+---------------------------+
|           App             |
| ESP RainMaker (Cloud/App) |
|  - Cards: Temp/Umid/Press |
|  - Card MQ2: Nivel,Estado |
|  - Card Alarme: Power     |
|  - Card Lâmpada: Power    |
+------------^--------------+
             |
             | MQTT/HTTPS (RMaker)
             v
+---------------------------+
|           ESP32           |
|  Tarefas:                 |
|  - Sensores (DHT,BMP180)  |
|  - MQ-2 (nivel/estado)    |
|  - LCD 16x2 (I²C)         |
|  - Lógica Alarme          |
|  - RMaker (provision/pub) |
+---^-----^--------^-----^--+
    |     |        |     |
    |     |        |     +--> GPIO23 (Lâmpada/SSR)
    |     |        +--------> GPIO33 (Alarme)
    |     +-----------------> I²C LCD 0x27
    +-----------------------> I²C BMP180 0x77
              +-------------> DHT22 GPIO4
              +-------------> MQ-2 AO -> GPIO34
```

## Dispositivos/Parâmetros (RainMaker)
- **Temperatura** (`esp.device.temperature-sensor`) → `Temperatura` (°C, R)
- **Umidade** (`esp.device.humidity-sensor`) → `Umidade` (%, R)
- **Pressão** (`esp.device.pressure-sensor`) → `Pressão` (hPa, R)
- **MQ2** (`esp.device.sensor`):
  - `Nivel` (%) (R)
  - `Estado` (`OK|ATENÇÃO|ALERTA`) (R)
  - `AlarmActive` (bool, R)
- **Alarme** (`esp.device.switch`):
  - `Power` (bool, R/W) — um clique
  - `Estado` (string, R) — detalhe
  - `AlarmActive` (bool, R) — detalhe
- **Lâmpada** (`esp.device.switch`) → `Power` (bool, R/W) — um clique

## Lógica do MQ‑2 e Alarme
- Converte AO→tensão→**Rs**→**ratio**≈`Rs/R0` e calcula **nível (%)** relativo ao ar limpo.
- Histerese:
  - `OK → ATENÇÃO`: `nivel >= warn_on` (padrão 20%)
  - `ATENÇÃO → ALERTA`: `nivel >= alert_on` (padrão 40%)
  - `ALERTA → ATENÇÃO`: `nivel < alert_off` (30%)
  - `ATENÇÃO → OK`: `nivel < warn_off` (15%)
- **AlarmActive** = `true` quando **Estado = ALERTA`.
- Saída **GPIO33** (alarme) = `Power` (do card Alarme) **E** `AlarmActive`.

## Estados no LCD
- Linha 1: `T:xx.x°C  U:yy.y%`
- Linha 2: `P:zzzz.z hPa` ou mensagens de erro (`DHT22 ERRO`, `BMP180 ERRO`).

## Particionamento e Provisionamento
- **Partition Scheme**: `RainMaker 4MB No OTA` (inclui `fctry` usado pelo RainMaker).
- **Provisionamento**: **SoftAP**, SSID `PROV_xxxxxx`, senha/POP `abcd1234`.

## Telemetria e *Throttling*
- Publicação com limites mínimos (10s) e *epsilon* por variável para economizar quota MQTT.
- Publicações de flags (AlarmActive/Estado) por **mudança de estado**.

## Segurança e Boas Práticas
- Não exponha a senha do Wi‑Fi no repositório.
- Gere um novo POP/senha para produção.
- Considere watchdogs e *brown‑out detection* ativos.
