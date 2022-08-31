#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <driver/uart.h>
#include <esp32/rom/ets_sys.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include <string.h>
#include "KRA_PID.h"
#include "PinNames.h"
#include "Ticker.h"
#include "ads1015.h"
#include "arm.h"
#include "commonfunc.h"
#include "gpio.h"
#include "i2c.h"
#include "mcpwm.h"
#include "motor.h"
#include "pca9557.h"
#include "servo.h"
#include "tof.h"
#include "twai.h"
#include "uart.h"

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

int stepCnt = 0;
int stepCycle = 6000;
int targetSpeed = 0;
int steps = 0;
int currentStep = 0;
const int minSpeed = 6000;
const int acceralationLimit = 400;

const int stpPeriod = 50;

Ticker ticker0;
Ticker ticker1;

gpio stp(Pe2A, OUTPUT);
gpio slp(E04, OUTPUT);
gpio dir(E05, OUTPUT);
servo servo0(Pe2B, 0, 360);
gpio s10(Pe2C, INPUT_PU);
gpio s11(Pe2D, INPUT_PU);
gpio user(USER, INPUT_PU);

bool prevVal = 0;
int targetStep = 0;

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
    stp.write((((stepCycle != 0) && (stepCycle != minSpeed)) && stepCnt < 100));
    if (prevVal == 1 && stp.read() == 0) {
        steps++;
        if (dir.read()) {
            currentStep++;
        } else {
            currentStep--;
        }
    }
    prevVal = stp.read();
}

void stepAc(void* pvParameters) {
    int limit;
    while (1) {
        if (targetStep <= acceralationLimit) {
            limit = targetStep / 2;
        } else {
            limit = acceralationLimit;
        }
        if (steps < limit) {
            stepCycle -= 2;
        } else if (steps >= targetStep) {
            stepCycle = 0;
        } else if (steps > targetStep - limit) {
            stepCycle += 4;
        }
        // if (targetSpeed == 0) {
        //     stepCycle = minSpeed;
        // } else if (stepCycle < targetSpeed) {
        //     stepCycle++;
        // } else if (stepCycle > targetSpeed) {
        //     stepCycle--;
        // }
        delay_ms(1);
    }
}

void stepAcc() {
    int limit;
    if (targetStep <= acceralationLimit) {
        limit = targetStep / 2;
    } else {
        limit = acceralationLimit;
    }
    if (steps < limit) {
        stepCycle -= 2;
    } else if (steps >= targetStep) {
        stepCycle = 0;
    } else if (steps > targetStep - limit) {
        stepCycle += 4;
    }
    // if (targetSpeed == 0) {
    //     stepCycle = minSpeed;
    // } else if (stepCycle < targetSpeed) {
    //     stepCycle++;
    // } else if (stepCycle > targetSpeed) {
    //     stepCycle--;
    // }
}

void stepToTarget(void* pvParameters) {
    // while(1){
    //     if(steps<200){
    //         targetSpeed--;
    //     }else if(steps>targetStep-200){
    //         targetSpeed++;
    //     }else if(steps>=targetStep){
    //         targetSpeed=0;
    //     }
    //     delay_ms(1);
    // }
}

void printStep(void* pvParameters) {
    while (1) {
        printf("%d\n", steps);
        delay_ms(1);
    }
}

void setSpeed(int speed, int step) {
    steps = 0;
    targetSpeed = speed;
    targetStep = step;
}

void setStep(int step) {
    stepCycle = minSpeed;
    steps = 0;
    dir.write(step > 0);
    // dir.write(step > 0);
    targetStep = abs(step);
}

void homeStp() {
    dir.write(1);
    stepCycle = 5000;
    while (1) {
        if (!user.read()) {
            break;
        }
        delay_ms(1);
    }
    stepCycle = minSpeed;
    dir.write(0);
    stepCycle = 5000;
    while (1) {
        if (!user.read()) {
            break;
        }
        delay_ms(1);
    }
    stepCycle = minSpeed;
    currentStep = 0;
}

void calPID() {
}

void app_main() {
    i2c.init();
    ex.set();
    uart.init();
    twai.init();
    slp.write(1);
    ticker0.attach_us(stpPeriod, step);
    // stepCycle=8000;
    // while (1) {
    //     delay_ms(10);
    // }
    homeStp();
    disableCore0WDT();

    // ticker1.attach_ms(pidPeriod,calA1PID);
    // xTaskCreatePinnedToCore(receiveTwai, "receiveTwai", 4096, NULL, 22, &taskHandle, 0);
    // xTaskCreatePinnedToCore(stepToTarget, "stepToTarget", 4096, NULL, 22, &taskHandle, 1);
    // xTaskCreatePinnedToCore(stepAc, "stepAc", 4096, NULL, 22, &taskHandle, 0);
    ticker1.attach_ms(1, stepAcc);

    // xTaskCreatePinnedToCore(printStep, "printStep", 4096, NULL, 21, &taskHandle, 0);
    // setSpeed(3000, 1000);
    stepCycle = minSpeed;
    dir.write(1);
    char buf[16];
    setStep(100);
    while (1) {
        printf("Enter Step\n");
        uart.read(buf);
        int target = 0;
        switch (atoi(buf)) {
            case 0:
                target = 0;
                break;
            case 1:
                target = 260;
                break;
            case 2:
                target = 350;
                break;
            case 3:
                target = 20;
                break;
            default:
                break;
        }
        setStep(target - currentStep);
        // setStep(atoi(buf));  //260 // 350ステップ 20
        printf("Step state is %d\n", atoi(buf));
        delay_ms(10);
    }
}
