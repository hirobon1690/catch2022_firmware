#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <driver/uart.h>
#include <esp32/rom/ets_sys.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include <string.h>
#include "PinNames.h"
#include "ads1015.h"
#include "commonfunc.h"
#include "gpio.h"
#include "i2c.h"
#include "mcpwm.h"
#include "motor.h"
#include "pca9557.h"
#include "tof.h"
#include "twai.h"
#include "uart.h"
#include "KRA_PID.h"
#include "arm.h"
#include "Ticker.h"
#include "servo.h"

extern "C" {
void app_main(void);
}

TaskHandle_t taskHandle;
char uart_msg[16];
float move_cmd[2];
float servo_angle;
int stepper_state;
int pump_state;
int emergency;

int stepCnt=0;
int stepCycle=0;
int targetSpeed=0;

const int stpPeriod=50;

Ticker ticker0;
Ticker ticker1;

gpio stp(Pe2A,OUTPUT);
gpio slp(E04,OUTPUT);
gpio dir(E05,OUTPUT);
servo servo0(Pe2B,0,360);
gpio s10(Pe2C, INPUT_PU);
gpio s11(Pe2D,INPUT_PU);

twai_message_t msg;

float unpackFloat(char* buf, int index) {
    return *(float*)(buf + index);
}

int unpackInt(char* buf, int index) {
    return *(buf + index);
}

void receiveTwai(void* pvParameters) {
    while (1) {
        twai.read(&msg);
        // printf("%d\n",msg.data);
    }
}


void step() {
    if (stepCnt >= stepCycle) {
        stepCnt = 0;
    } else {
        stepCnt += 50;
    }
    stp.write((stepCycle != 6000 && stepCnt < 100));
}

void stepAc(void* pvParameters) {
    while (1) {
        if(stepCycle<targetSpeed){
            stepCycle++;
            delay_ms(1);
        }else if(stepCycle>targetSpeed){
            stepCycle--;
            delay_ms(1);
        }
    }
}

void setSpeed(int speed){
    targetSpeed=speed;
}

void homeStp(){
    dir.write(0);
    stepCnt=2000;
     while (1) {
        if (!s10.read()) {
            break;
        }
        delay_ms(1);
    }
    stepCnt=0;
    dir.write(1);
    stepCnt=2000;
    while (1) {
        if (!s11.read()) {
            break;
        }
        delay_ms(1);
    }
    stepCnt=0;
}

void calPID(){
    
}

void app_main() {
    i2c.init();
    ex.set();
    uart.init();
    twai.init();
    slp.write(1);
    // ticker0.attach_us(stpPeriod,step);
    // ticker1.attach_ms(pidPeriod,calA1PID);
    // xTaskCreatePinnedToCore(receiveTwai, "receiveTwai", 4096, NULL, 22, &taskHandle, 0);
    // xTaskCreatePinnedToCore(receiveTwai, "stepAc", 4096, NULL, 22, &taskHandle, 1);
    setSpeed(3000);
    while (1) {
        
        delay_ms(10);
    }
}
