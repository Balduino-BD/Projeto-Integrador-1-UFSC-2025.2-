# INSTALL.md — Guia de Instalação

> **Resumo**: este guia configura o ambiente de desenvolvimento, bibliotecas e partições, grava o firmware e realiza o provisionamento **SoftAP** do ESP RainMaker.

---

## 1) Pré‑requisitos
- **Arduino IDE** atualizado.
- **ESP32 core (Espressif)** instalado via Gerenciador de Placas.
- Cabos e drivers USB (CP210x/CH34x) instalados.

## 2) Bibliotecas necessárias
Instale via **Gerenciador de Bibliotecas** (ou ZIP):
- `ESP RainMaker` (RMaker)
- `WiFiProv`
- `Adafruit BMP085 Library` (para **BMP180**)
- `DHT sensor library`
- `LiquidCrystal_I2C`

## 3) Configuração da Placa
Ferramentas → selecione **ESP32 Dev Module** (ou sua variante) e ajuste:
- **Partition Scheme**: `RainMaker 4MB No OTA` (recomendado).
- (Opcional, 1ª gravação após mudar partição): **Erase All Flash Before Sketch Upload → All Flash Contents**.

## 4) Gravação do firmware
- Abra o sketch `.ino` do projeto.
- Clique **Upload**.
- Monitore a **Serial** (115200) para checar mensagens de boot e sensores.

## 5) Provisionamento (SoftAP ou BLE)
O firmware já usa **SoftAP** para economizar Flash.
- Ao iniciar, o ESP32 cria o AP `PROV_xxxxxx` (gera a partir do MAC).
- **Senha/POP**: `abcd1234`.(Obs: Temporário)
- No app **ESP RainMaker**, escolha **Provision via SoftAP**, conecte-se ao SSID e conclua.

> Se preferir BLE, altere no código a chamada `WiFiProv.beginProvision(...)` para `NETWORK_PROV_SCHEME_BLE` e regrave o firmware (o binário pode aumentar).

## 6) Verificação rápida
- Cartões visíveis no app (PT‑BR): **Temperatura**, **Umidade**, **Pressão**, **MQ2**, **Alarme**, **Lâmpada**.
- Clique em **Alarme** ou **Lâmpada** para ligar/desligar (um clique).
- Em **ALERTA** (MQ‑2), o **Alarme** liga automaticamente se o Power estiver **ON**.

---

## 7) Troubleshooting (detalhado)
- **NVS / RainMaker fctry**:
  1. Ferramentas → `Partition Scheme: RainMaker 4MB No OTA`.
  2. Habilite **Erase All Flash** (apenas na primeira gravação).
  3. Faça o upload novamente.
- **Sketch muito grande**: mantenha SoftAP; desative *debugs* extensos no código; confirme partição correta.
- **BMP180 não responde**: confira I²C (GPIO21/22), tensão **3V3**, cabo, e o endereço 0x77.
- **MQ‑2** nivel instável**: aguarde aquecimento do sensor; verifique alimentação e fiação do AO → GPIO34.
- **Porta COM ocupada**: feche Serial Monitor, VS Code, Edge Impulse, PuTTY; reconecte USB e verifique `Gerenciador de Dispositivos`.

---

## 8) Checklist final
- [ ] Partição `RainMaker 4MB No OTA` selecionada.
- [ ] Bibliotecas instaladas.
- [ ] Firmware gravado sem erros.
- [ ] SoftAP provisionado (`abcd1234`). (Obs: Temporário)
- [ ] Cartões operacionais no app.
