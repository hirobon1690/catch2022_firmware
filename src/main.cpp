#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp32/rom/ets_sys.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "PinNames.h"
#include "gpio.h"
#include "gpioex.h"

gpio LED(E00,INPUT_PU);
gpio LED2(E11,OUTPUT);

extern "C" {
void app_main(void);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    i2c.init();
    ex.set();
    // zero-initialize the config structure.
    // bool level = 0;
    // LED=1;
    LED2=1;
    while (1) {
        // LED.flip();
        LED2.flip();
        printf("%d\n",bool(LED));
        delay_ms(500);
    }
}
