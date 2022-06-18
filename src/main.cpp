#include <driver/adc.h>
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
    adc2_config_channel_atten(ADC2_CHANNEL_2, ADC_ATTEN_DB_11);
    int result = 0;

    while (1) {
        adc2_get_raw(ADC2_CHANNEL_2, ADC_WIDTH_BIT_12, &result);
        printf("%d\n", result);
        vTaskDelay(1);
    }
}
