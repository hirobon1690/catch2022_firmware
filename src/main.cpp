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

extern "C" {
void app_main(void);
}

gpio dir(E05, OUTPUT);
gpio slp(E04, OUTPUT);
gpio stp(P18,OUTPUT);
gpio pmp0(E06,OUTPUT);
gpio pmp1(E01,OUTPUT);
gpio vlv0(E02,OUTPUT);

void delay_ms(int ms) {
    vTaskDelay(ms/portTICK_PERIOD_MS);
}



void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    slp.write(1);
    dir.write(0);
    stp.write(0);
    vlv0.write(1);
    pmp0.write(1);
    pmp1.write(1);
    printf("OK\n");
    while(1){
        pmp0.write(1);
        pmp1.write(1);
        delay_ms(100);
    }
}
