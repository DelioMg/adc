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
#define VRX_PIN 26  
#define VRY_PIN 27  
#define SW_PIN 22   
#define LED_R 11   
#define LED_G 12   
#define LED_B 13   
#define BTN_A 5   

volatile bool toggle_led = false;
volatile bool toggle_display_border = false;
volatile bool toggle_pwm = true;

void button_callback(uint gpio, uint32_t events) {
    if (gpio == SW_PIN) {
        toggle_led = !toggle_led;
    } else if (gpio == BTN_A) {
        toggle_pwm = !toggle_pwm;
    }
}

void init_hardware(ssd1306_t *ssd) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
    
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
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
    int x = (vrx_value * 120) / 4095;
    int y = (vry_value * 56) / 4095;
    
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, x, y, 8, 8, true, false);
    if (toggle_display_border) {
        ssd1306_rect(ssd, 0, 0, 128, 64, true, false);
    }
    ssd1306_send_data(ssd);
}

int main() {
    ssd1306_t ssd;
    init_hardware(&ssd);
    while (true) {
        adc_select_input(0);
        uint16_t vrx_value = adc_read();
        adc_select_input(1);
        uint16_t vry_value = adc_read();
        
        if (toggle_pwm) {
            pwm_set_gpio_level(LED_R, abs(vrx_value - 2048) * 2);
            pwm_set_gpio_level(LED_B, abs(vry_value - 2048) * 2);
        } else {
            pwm_set_gpio_level(LED_R, 0);
            pwm_set_gpio_level(LED_B, 0);
        }
        
        update_display(&ssd, vrx_value, vry_value);
        sleep_ms(100);
    }
    return 0;
}
