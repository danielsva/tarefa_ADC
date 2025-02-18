# Controle de LED e Display com Joystick

## 📷 Demonstração

- **Execução na Placa BitDogLab:** [Assista no YouTube](https://youtu.be/QZhPXDIq2H0](https://youtu.be/pACJTYoGSQQ)

## 📌 Sobre o Projeto
Este projeto utiliza um **joystick analógico** para controlar LEDs com **PWM** e um **display OLED SSD1306**. O sistema ajusta o brilho dos LEDs conforme os eixos X e Y do joystick, além de exibir um quadrado no display que se move proporcionalmente.

### 🎮 Funcionalidades
✅ **Movimento do quadrado no display** conforme a posição do joystick
✅ **Ajuste do brilho dos LEDs** baseado nos valores dos eixos X e Y
✅ **Botão do Joystick (PB):**
   - Alterna o estado do **LED Verde**
   - Modifica o estilo da **borda do display**
✅ **Botão A:** Ativa ou desativa os LEDs PWM

---

## 🛠️ Hardware Utilizado
- **Raspberry Pi Pico**
- **Joystick analógico**
- **Display OLED SSD1306 (I2C)**
- **LEDs (Vermelho, Azul e Verde)**
- **Resistores**
- **Botões**

---

## 📜 Código Principal
O código está no arquivo `main.c`, onde:
- **Leitura do joystick** ocorre via ADC
- **LEDs Azul e Vermelho** têm brilho ajustado via PWM
- **Botão do Joystick** alterna LED Verde e a borda do display
- **Botão A** liga/desliga os LEDs PWM
- **Movimento do quadrado** no display é atualizado dinamicamente

---

## 🔌 Conexões
| Componente | Pino Raspberry Pi Pico |
|------------|-----------------------|
| Joystick X | GP26 (ADC0) |
| Joystick Y | GP27 (ADC1) |
| Botão Joystick (PB) | GP22 |
| Botão A | GP5 |
| LED Azul | GP12 (PWM) |
| LED Vermelho | GP13 (PWM) |
| LED Verde | GP11 |
| Display SSD1306 SDA | GP14 |
| Display SSD1306 SCL | GP15 |

---

## 🚀 Como Executar
1️⃣ Conecte o Raspberry Pi Pico ao computador
2️⃣ Compile o código e transfira o `.uf2` para o Pico
3️⃣ Observe o comportamento no display e LEDs

---

