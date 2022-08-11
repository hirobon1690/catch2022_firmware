#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <driver/uart.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include <string.h>
#include "PinNames.h"
#include "Ticker.h"
#include "ads1015.h"
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
    vTaskDelay(ms / portTICK_RATE_MS);
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
        // stp.flip();
        delay_ms(10);
        char sample[128];
        printf("Enter Stepcycle\n");
        uart.read(sample);
        stepCycle = atoi(sample);
        printf("\nCycle is %d\n", stepCycle);
        // servo.duty(angle);
        // delay_ms(10);
    }
}
