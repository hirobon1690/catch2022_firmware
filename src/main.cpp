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
#include "i2c.h"
#include "pca9557.h"
#include "ads1015.h"
#include "tof.h"

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);

void delay_ms(int ms) {
    vTaskDelay(ms/portTICK_RATE_MS);
}



void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    
    printf("init\n");
    while (1) {
        
    }
}
