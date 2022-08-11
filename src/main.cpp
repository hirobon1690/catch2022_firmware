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
#include "mcpwm.h"
#include "pca9557.h"
#include "uart.h"

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);

mcpwm pwm0(P14, MCPWM_UNIT_0, MCPWM0A);

void enableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void disableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    disableCore0WDT();
    uart.init();
    dir0.write(1);
    dir1.write(0);

    dir0.write(1);
    dir1.write(0);

    printf("init\n");
    int duty = 0;
    while (1) {
        dir0.write(1);
        dir1.write(0);
        pwm0.write(100);

        // delay_ms(10);
        // char sample[128];
        // printf("Enter duty\n");
        // uart.read(sample);
        // duty = atoi(sample);
        // pwm0.write(duty);
        // printf("Duty is %d\n", duty);

        // dir0.write(1);
        // dir1.write(0);
        // for (float i = 0; i < 100; i += 0.05) {
        //     pwm0.write(i);
        //     printf("%f\n", i);
        //     // delay_ms(2);
        // }
        // for (float i = 100; i > 0; i -= 0.05) {
        //     pwm0.write(i);
        //     printf("%f\n", i);
        //     // delay_ms(2);
        // }
        // dir0.write(0);
        // dir1.write(1);
        // for (float i = 0; i < 100; i += 0.05) {
        //     pwm0.write(i);
        //     printf("%f\n", i);
        //     // delay_ms(2);
        // }
        // for (float i = 100; i > 0; i -= 0.05) {
        //     pwm0.write(i);
        //     printf("%f\n", i);
        //     // delay_ms(2);
        // }
    }
}
