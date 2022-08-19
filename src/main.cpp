#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include "PinNames.h"
#include "Ticker.h"
#include "ads1015.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "i2c.h"
#include "mcpwm.h"
#include "pca9557.h"
#include "servo.h"
#include "stepper.h"
#include "tof.h"
#include "uart.h"

extern "C" {
void app_main(void);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    i2c.init();
    ex.set();
    gpio pin(Pe2C,OUTPUT);
    tof tof1(pin);
    // tof tof2(Pe2D);

    while(1){
        int dist;
        dist=tof1.read();
        printf("%d\n",dist);
        delay_ms(100);
    }
}
