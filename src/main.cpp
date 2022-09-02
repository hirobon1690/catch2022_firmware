#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include "PinNames.h"
#include "Ticker.h"
#include "ads1015.h"
#include "commonfunc.h"
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

int rawData[2] = {0, 0};

void app_main() {
    i2c.init();
    ex.set();
    gpio pin0(Pe2A, OUTPUT);
    gpio pin1(Pe2B, OUTPUT);
    gpio pin2(Pe2C, OUTPUT);
    gpio pin3(Pe2D, OUTPUT);
    tof tof1(pin0);
    pin0.write(0);
    pin1.write(0);
    pin2.write(0);
    pin3.write(0);
    tof tof0(pin0);
    // tof tof1(pin2);
    // tof tof2(Pe2D);
    
    while (1) {
        // pin0.flip();
        // pin1.flip();
        // pin2.flip();
        // pin3.flip();
        // delay_ms(500);
        pin0.write(1);
        int dist;
        dist=tof0.read();
        printf("%d\n",dist);
        delay_ms(100);
    }
}
