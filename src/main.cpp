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
#include "Ticker.h"
#include "stepper.h"
#include "ads1015.h"
#include "tof.h"

extern "C" {
void app_main(void);
}

gpio dir(E05, OUTPUT);
gpio slp(E04, OUTPUT);
gpio stp(P18,OUTPUT);
gpio pmp0(E06,OUTPUT);
gpio pmp1(E01,OUTPUT);
gpio vlv0(E02,OUTPUT);

Ticker ticker;
int stepCnt=0;
int stepCycle=20;
void step(){
    if(stepCnt>=stepCycle){
        stepCnt=0;
    }else{
        stepCnt+=50;
    }
    stp=(stepCycle!=0&&stepCnt<100);
}

void delay_ms(int ms) {
    vTaskDelay(ms/portTICK_RATE_MS);
}



void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    slp.write(1);
    dir.write(0);
    stp.write(0);
    ticker.attach_us(50,step);
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
