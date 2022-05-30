#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp32/rom/ets_sys.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "PinNames.h"
#include "gpio.h"

gpio LED(P25,OUTPUT);

// typedef gpio_num_t PinName;
extern "C" {
void app_main(void);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    // zero-initialize the config structure.
    bool level = 0;
    LED=1;
    while (1) {
        LED=!LED;
        delay_ms(500);
    }
}
