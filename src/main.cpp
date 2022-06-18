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
#include "gpioex.h"
#include "i2c.h"

extern "C" {
void app_main(void);
}

gpio dir0(E12, OUTPUT);
gpio dir1(E11, OUTPUT);

void delay_ms(int ms) {
    ets_delay_us(ms * 1000);
}

void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    dir0.write(1);
    dir1.write(0);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, (int)P25);
    mcpwm_config_t pwmconfig;
    pwmconfig.frequency = 10000;
    pwmconfig.cmpr_a = 0;  // duty cycle of PWMxA = 0
    pwmconfig.counter_mode = MCPWM_UP_COUNTER;
    pwmconfig.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwmconfig);
    printf("init\n");
    while (1) {
        for (float i=0; i < 100; i += 0.5) {
            mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
            delay_ms(500);
            vTaskDelay(1);
            printf("%f\n",i);
        }
        for (float i=100; i > 0; i -= 0.5) {
            mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
            delay_ms(500);
            vTaskDelay(1);
            printf("%f\n",i);
        }
    }
}
