#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "lib/ssd1306.h"
#include "lib/font.h"


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22 // GPIO para botão do Joystick
#define Botao_A 5 // GPIO para botão A
#define LED_VERMELHO 13
#define LED_AZUL 12
#define LED_VERDE 11


void configurar_pwm(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);  // Configura o pino como PWM
  uint slice = pwm_gpio_to_slice_num(pin); // Obtém o slice do PWM para o pino
  pwm_set_wrap(slice, 4095); // Define o valor máximo do contador PWM
  pwm_set_chan_level(slice, pwm_gpio_to_channel(pin), 0); // Inicializa com 0% de duty cycle
  pwm_set_enabled(slice, true); // Habilita o PWM
}


int main()
{
  //Inicializa os LEDs, Joystick e botão A
  gpio_init(JOYSTICK_PB);
  gpio_set_dir(JOYSTICK_PB, GPIO_IN);
  gpio_pull_up(JOYSTICK_PB); 

  gpio_init(Botao_A);
  gpio_set_dir(Botao_A, GPIO_IN);
  gpio_pull_up(Botao_A);

  gpio_init(LED_AZUL);
  gpio_set_dir(LED_AZUL, GPIO_OUT);

  gpio_init(LED_VERDE);
  gpio_set_dir(LED_VERDE, GPIO_OUT);
  gpio_put(LED_VERDE, 0); // Inicialmente apagado

  gpio_init(LED_VERMELHO);
  gpio_set_dir(LED_VERMELHO, GPIO_OUT);

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  //Inicializando o ADC
  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN); 
  adc_select_input(1);
  
  // Configurar LEDs como PWM
  configurar_pwm(LED_AZUL);
  configurar_pwm(LED_VERMELHO);

  uint slice_azul = pwm_gpio_to_slice_num(LED_AZUL);
  uint slice_vermelho = pwm_gpio_to_slice_num(LED_VERMELHO);
  
  uint16_t adc_value_x;
  uint16_t adc_value_y;  
  char str_x[5];  // Buffer para armazenar a string
  char str_y[5];  // Buffer para armazenar a string  
  
  // Variáveis para o quadrado
  int quadrado_x = 60, quadrado_y = 28;
  bool estado_led_verde = false;
  bool pwm_ativo = true;
  int estilo_borda = 0;
  bool botao_pb_pressionado = false;
  bool botao_a_pressionado = false;

  while (true)
  {
    //Leitura do joystick

    adc_select_input(0); // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
    adc_value_x = adc_read();
    adc_select_input(1); // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
    adc_value_y = adc_read(); 

    // Converte valores ADC para posição do quadrado (ajuste para centralizar)
    quadrado_x = (adc_value_x * 112) / 4095 + 8; // De 8 a 120 pixels
    quadrado_y = (adc_value_y * 48) / 4095 + 8;  // De 8 a 56 pixels

    
    // Converte o valor do ADC (0-4095) para duty cycle (0-100%)
    uint16_t duty_cycle_y = abs(2048 - adc_value_y) * 2; // Ajusta a intensidade
    uint16_t duty_cycle_x = abs(2048 - adc_value_x) * 2; // Ajuste proporcional

    
    if (pwm_ativo) {
      pwm_set_chan_level(slice_vermelho, pwm_gpio_to_channel(LED_VERMELHO), duty_cycle_x);
      pwm_set_chan_level(slice_azul, pwm_gpio_to_channel(LED_AZUL), duty_cycle_y);
  } else {
      pwm_set_chan_level(slice_vermelho, pwm_gpio_to_channel(LED_VERMELHO), 0);
      pwm_set_chan_level(slice_azul, pwm_gpio_to_channel(LED_AZUL), 0);
  }

  // Detecta o botão do joystick (PB) pressionado e alterna o LED Verde e a borda
  if (gpio_get(JOYSTICK_PB) == 0 && !botao_pb_pressionado) {
    botao_pb_pressionado = true;
    estado_led_verde = !estado_led_verde;
    gpio_put(LED_VERDE, estado_led_verde);
    estilo_borda = (estilo_borda + 1) % 3;
  }
  if (gpio_get(JOYSTICK_PB) == 1) {
      botao_pb_pressionado = false;
  }

  // Detecta o botão A pressionado e alterna os LEDs PWM
  if (gpio_get(Botao_A) == 0 && !botao_a_pressionado) {
    botao_a_pressionado = true;
    pwm_ativo = !pwm_ativo;
  }
  if (gpio_get(Botao_A) == 1) {
      botao_a_pressionado = false;
  }

  // Atualiza o display
  ssd1306_fill(&ssd, false);
  
  // Desenha a borda conforme o estilo atual
  if (estilo_borda == 0) {
    ssd1306_rect(&ssd, 3, 3, 122, 60, true, false); // Borda normal
  } else if (estilo_borda == 1) {
      ssd1306_line(&ssd, 3, 3, 122, 3, true); // Borda superior
      ssd1306_line(&ssd, 3, 60, 122, 60, true); // Borda inferior
  } else if (estilo_borda == 2) {
      ssd1306_rect(&ssd, 3, 3, 122, 60, true, true); // Borda invertida
  }

  // Desenha o quadrado no display
  ssd1306_rect(&ssd, quadrado_x, quadrado_y, 8, 8, true, true);

  // Mostra os valores do joystick
  char str_x[6], str_y[6];
  sprintf(str_x, "%d", adc_value_x);
  sprintf(str_y, "%d", adc_value_y);
  ssd1306_draw_string(&ssd, "X:", 10, 30);
  ssd1306_draw_string(&ssd, str_x, 30, 30);
  ssd1306_draw_string(&ssd, "Y:", 10, 45);
  ssd1306_draw_string(&ssd, str_y, 30, 45);

  ssd1306_send_data(&ssd);
  sleep_ms(50);
 }

}