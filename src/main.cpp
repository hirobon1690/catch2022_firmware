#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include "PinNames.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "i2c.h"
#include "pca9557.h"

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);

void delay_ms(int ms) {
    ets_delay_us(ms * 1000);
}

void app_main() {
    

    delay_ms(10);
    i2c.init();
    ex.set();
    dir0.write(1);
    dir1.write(0);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, (int)P14);
    mcpwm_config_t pwmconfig;
    pwmconfig.frequency = 100000;
    pwmconfig.cmpr_a = 0;  // duty cycle of PWMxA = 0
    pwmconfig.counter_mode = MCPWM_UP_COUNTER;
    pwmconfig.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwmconfig);
    mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, 10000);
    printf("init\n");
    while (1) {
        dir0.write(1);
        dir1.write(0);
        for (float i = 0; i < 100; i += 0.01) {
            mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
            // delay_ms(10);
            vTaskDelay(1 / portTICK_RATE_MS);
            printf("%f\n", i);
        }
        for (float i = 100; i > 0; i -= 0.01) {
            mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
            // delay_ms(10);
            vTaskDelay(1 / portTICK_RATE_MS);
            printf("%f\n", i);
        }
        dir0.write(0);
        dir1.write(1);
        for (float i = 0; i < 100; i += 0.01) {
            mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
            // delay_ms(10);
            vTaskDelay(1 / portTICK_RATE_MS);
            printf("%f\n", i);
        }
        for (float i = 100; i > 0; i -= 0.01) {
            mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
            // delay_ms(10);
            vTaskDelay(1 / portTICK_RATE_MS);
            printf("%f\n", i);
        }
    }
}
