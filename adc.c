#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define VRX_PIN 27  
#define VRY_PIN 26  
#define SW_PIN 22   // Mantido o pino 22 para o botão do joystick
#define LED_R 13   
#define LED_G 11   
#define LED_B 12 
#define BTN_A 5    
#define BTN_J 22   // Alterado para pino 22

volatile bool toggle_led = false;
volatile bool toggle_display_border = false;
volatile bool toggle_pwm = true;  // Inicializa com PWM ativado
volatile bool led_green_on = false; // Estado do LED verde

// Função callback para os botões
void button_callback(uint gpio, uint32_t events) {
    if (gpio == SW_PIN) {
        toggle_led = !toggle_led;  // Alterna o LED quando o SW_PIN for pressionado
    } else if (gpio == BTN_A) {
        toggle_pwm = !toggle_pwm;  // Alterna o estado do controle PWM quando o BTN_A for pressionado
    } else if (gpio == BTN_J) {
        led_green_on = !led_green_on;  // Alterna o LED verde
        toggle_display_border = !toggle_display_border;  // Alterna o estilo da borda
    }
}

void init_hardware(ssd1306_t *ssd) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
    
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);  // Ativa o resistor de pull-up
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);  // Ativa o resistor de pull-up
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    gpio_init(BTN_J);
    gpio_set_dir(BTN_J, GPIO_IN);
    gpio_pull_up(BTN_J);  // Ativa o resistor de pull-up
    gpio_set_irq_enabled_with_callback(BTN_J, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    gpio_set_function(LED_R, GPIO_FUNC_PWM);
    gpio_set_function(LED_G, GPIO_FUNC_PWM);
    gpio_set_function(LED_B, GPIO_FUNC_PWM);
    
    uint slice_r = pwm_gpio_to_slice_num(LED_R);
    uint slice_g = pwm_gpio_to_slice_num(LED_G);
    uint slice_b = pwm_gpio_to_slice_num(LED_B);
    pwm_set_wrap(slice_r, 4095);
    pwm_set_wrap(slice_g, 4095);
    pwm_set_wrap(slice_b, 4095);
    pwm_set_enabled(slice_r, true);
    pwm_set_enabled(slice_g, true);
    pwm_set_enabled(slice_b, true);
    
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_init(ssd, 128, 64, true, endereco, I2C_PORT);
    ssd1306_config(ssd);
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

void update_display(ssd1306_t *ssd, uint16_t vrx_value, uint16_t vry_value) {
    int y = ((vry_value - 2048) * 54) / 4095 + 28;  
    int x = ((vrx_value - 2048) * 115) / 4095 + 60 ;  
    
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, y, x, 8, 8, true, false);
    
    // Alterna a borda do display
    if (toggle_display_border) {
        // Borda mais espessa
        ssd1306_rect(ssd, 0, 0, 128, 64, true, false);
        ssd1306_rect(ssd, 2, 2, 124, 60, true, false);
    } else {
        // Borda simples
        ssd1306_rect(ssd, 0, 0, 128, 64, true, false);
    }
    
    ssd1306_send_data(ssd);
}

int main() {
    ssd1306_t ssd;
    init_hardware(&ssd);
    while (true) {
        adc_select_input(0);
        uint16_t vry_value = adc_read();
        adc_select_input(1);
        uint16_t vrx_value = adc_read();
        
        // Controla o LED verde com base no estado alternado
        if (led_green_on) {
            pwm_set_gpio_level(LED_G, 4095);  
        } else {
            pwm_set_gpio_level(LED_G, 0);     
        }
        
        if (toggle_pwm) {
            pwm_set_gpio_level(LED_R, abs(vrx_value - 2048) * 2);  // LED vermelho controlado pelo joystick
            pwm_set_gpio_level(LED_B, abs(vry_value - 2048) * 2);  // LED azul controlado pelo joystick
        } else {
            // Desliga LED azul
            pwm_set_gpio_level(LED_R, 0);  
            pwm_set_gpio_level(LED_B, 0);  
        }
        
        update_display(&ssd, vrx_value, vry_value);  // Atualiza a tela com o joystick
        sleep_ms(100);
    }
    return 0;
}
