# Projeto de Controle  com Joystick

Este projeto implementa o controle de um robô utilizando um joystick com um microcontrolador, exibindo informações no display OLED e controlando LEDs via PWM. O sistema também permite alternar o estilo da borda no display e o controle de LEDs através de botões.

## Funcionalidades

- **Joystick**: Controla a posição no display e ajusta a intensidade dos LEDs (vermelho e azul) via PWM.
- **Botões**:
  - **Botão A**: Ativa/desativa o controle PWM.
  - **Botão do Joystick**: Alterna o estado do LED verde e o estilo da borda do display OLED.
  - **Botão SW**: Alterna o estado do LED (vermelho, verde, azul).
- **Display OLED**: Exibe a posição do joystick e o estado dos LEDs.
- **Controle de LEDs**: Utiliza PWM para controlar a intensidade dos LEDs vermelho, verde e azul.

## Requisitos

- Raspberry Pi Pico ou qualquer placa compatível com o RP2040.
- Display OLED SSD1306.
- Joystick analógico com botões.
- LEDs RGB (vermelho, verde, azul).
- Biblioteca `ssd1306` para o controle do display.
- Biblioteca `hardware/pwm`, `hardware/adc`, e `hardware/i2c` para o controle de hardware.

## Conexões

| Componente        | Pino do Raspberry Pi Pico |
|-------------------|---------------------------|
| **Joystick X**     | Pino 27                   |
| **Joystick Y**     | Pino 26                   |
| **Botão SW**       | Pino 22                   |
| **Botão A**        | Pino 5                    |
| **Botão Joystick** | Pino 22                   |
| **LED Vermelho**   | Pino 13                   |
| **LED Verde**      | Pino 11                   |
| **LED Azul**       | Pino 12                   |
| **SDA (I2C)**      | Pino 14                   |
| **SCL (I2C)**      | Pino 15                   |

## Como Usar

1. Clone este repositório para sua máquina:
   ```bash
   git clone https://github.com/DelioMg/adc.git

2. Compile e carregue o código no seu microcontrolador Raspberry Pi Pico.

3. Conecte os componentes conforme as especificações de pinos acima.

4. Execute o programa e interaja com o joystick e os botões.

# Vídeo
Assista ao vídeo do funcionamento do projeto:

