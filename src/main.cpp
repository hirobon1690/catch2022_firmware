#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include "PinNames.h"
#include "Ticker.h"
#include "VL53L0X.h"
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

#include "VL53L0X.h"
VL53L0X sensor;
VL53L0X sensor1;

extern "C" {
void app_main(void);
}
gpio pin0(Pe1A, OUTPUT);
gpio pin1(Pe1C, OUTPUT);
void app_main() {
    /* initialization */
    i2c.init();
    pin0.write(1);
    sensor.init();
    sensor.setAddress(0x30);
    pin1.write(1);
    sensor1.init();
    while (1) {
        int dist[2];
        dist[0] = sensor.readRangeSingleMillimeters();
        dist[1] = sensor1.readRangeSingleMillimeters();
        printf("%d, %d\n", dist[0],dist[1]);
        delay_ms(100);
    }
}

// void delay_ms(int ms) {
//     vTaskDelay(ms / portTICK_RATE_MS);
// }

// int rawData[2] = {0, 0};
// gpio sensor0(Pe2A, OUTPUT);
// gpio sensor1(Pe2C, OUTPUT);
// VL53L0X sensor;

// void app_main() {
//     i2c.init();
//     ex.set();
//     sensor0.write(1);
//     sensor1.write(1);
//     // if (!sensor.init()) {
//     //     printf("Failed to detect and initialize sensor!\n");
//     //     while (1) {
//     //         delay_ms(100);
//     //     }
//     // }
//     // gpio pin0(Pe2A, OUTPUT);
//     // gpio pin1(Pe2B, OUTPUT);
//     // gpio pin2(Pe2C, OUTPUT);
//     // gpio pin3(Pe2D, OUTPUT);
//     // tof tof1(pin0);
//     // pin0.write(0);
//     // pin1.write(0);
//     // pin2.write(0);
//     // pin3.write(0);
//     // tof tof0(pin0);
//     // tof tof1(pin2);
//     // tof tof2(Pe2D);
//     // vl53l0x_setTimeout(sensor, (uint16_t)500);
//     // if (vl53l0x_init(sensor)) {
//     //     printf("Failed to detect and initialize sensor!");
//     //     while (1) {
//     //     }
//     // }
//     int dist = 0;
//     while (1) {
//         // pin0.flip();
//         // pin1.flip();
//         // pin2.flip();
//         // pin3.flip();
//         // delay_ms(500);
//         // pin0.write(1);
//         // int dist;
//         // dist=tof0.read();

//         delay_ms(100);
//     }
// }
