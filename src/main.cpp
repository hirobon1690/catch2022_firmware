#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp32/rom/ets_sys.h>
#include <stdio.h>
#include "PinNames.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "gpioex.h"
#include "i2c.h"

gpio pwm1(P25,OUTPUT);
gpio dir0(E12,OUTPUT);
gpio dir1(E11,OUTPUT);

extern "C" {
    void app_main(void);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    pwm1.write(0);
    dir0.write(0);
    dir1.write(1);
    while (1) {
        pwm1.write(1);
        delay_ms(1);
        // pwm1.write(0);
        // delay_ms(1);
    }
}
