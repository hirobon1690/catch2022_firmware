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

const int pidPeriod = 10;

motor m0(Pe1A, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, E04, E01);
motor m1(Pe1B, MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM1A, E03, E02);
gpio s00(Pe1C, INPUT_PU);
gpio s01(Pe1D, INPUT_PU);
gpio s10(Pe2A, INPUT_PU);
gpio s11(Pe2B, INPUT_PU);
gpio user(USER, INPUT_PU);
gpio pmp[2] = {gpio(Pe0A, OUTPUT), gpio(Pe0B, OUTPUT)};
gpio vlv[2] = {gpio(Pe0C, OUTPUT), gpio(Pe0D, OUTPUT)};
adc pot0(A2);
adc pot1(A3);
arm a0(m0, s00, s01, pot0, 250);
arm a1(m1, s10, s11, pot1, 276);
KRA_PID pid0(pidPeriod, 0, 250, 0, 15);
KRA_PID pid1(pidPeriod, 0, 276, 0, 30);

Ticker ticker0;
Ticker ticker1;

float unpackFloat(char* buf, int index) {
    return *(float*)(buf + index);
}

int unpackInt(char* buf, int index) {
    return *(buf + index);
}

void turnPmp(bool val) {
    for (int i = 0; i < 2; i++) {
        pmp[i].write(val);
        vlv[i].write(val);
    }
}

void receiveUart(void* pvParameters) {
    while (1) {
        uart_read_bytes(UART_NUM_0, uart_msg, 16, portMAX_DELAY);
        if (uart_msg[15] != 0xFF) {
            printf("Connection Failed!\n");
            while (1) {
                char buf[1];
                uart_read_bytes(UART_NUM_0, buf, 1, portMAX_DELAY);
                if (buf[0] == 0xFF) {
                    break;
                }
            }
        }
        move_cmd[0] = unpackFloat(uart_msg, 0);
        move_cmd[1] = unpackFloat(uart_msg, 4);
        pid0.setgoal(move_cmd[0]);
        pid1.setgoal(move_cmd[1]);
        servo_angle = unpackFloat(uart_msg, 8);
        stepper_state = unpackInt(uart_msg, 12);
        pump_state = (bool)unpackInt(uart_msg, 13);
        turnPmp(pump_state);
        emergency = unpackInt(uart_msg, 14);
    }
}

void calPID() {
    xTaskNotify(taskHandle, 0, eNoAction);
    // float currentDeg[2] = {0, 0};
    // while (1) {
        // currentDeg[0] = a0.getDeg();
        // delay_ms(2);
        // currentDeg[1] = a1.getDeg();
        // delay_ms(2);
        // m0.write(pid0.calPID(currentDeg[0]));
        // m1.write(pid1.calPID(currentDeg[1]));
        // printf("%f, %f\n", currentDeg[0], currentDeg[1]);
        // delay_ms(8);
    // }
}

void adctest(void* pvParameters){
    float currentDeg[2] = {0, 0};
    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        currentDeg[0] = a0.getDeg();
        delay_ms(3);
        currentDeg[1] = a1.getDeg();
        // delay_ms(3);
        m0.write(pid0.calPID(currentDeg[0]));
        // m1.write(pid1.calPID(currentDeg[1]));
        printf("%3.2f, %3.2f\n", currentDeg[0], currentDeg[1]);
    }
}

void app_main() {
    i2c.init();
    ex.set();
    uart.init();
    twai.init();
    int result[2];
    pid0.setgain(10, 0, 0);
    pid1.setgain(10, 0, 0);
    a0.home(15);
    // a1.home(30);
    pid0.setgoal(125);
    ticker0.attach_ms(pidPeriod, calPID);

    // ticker1.attach_ms(pidPeriod,calA1PID);
    xTaskCreatePinnedToCore(receiveUart, "receiveUart", 4096, NULL, 22, &taskHandle, 0);
    xTaskCreatePinnedToCore(adctest, "adctest", 4096, NULL, 22, &taskHandle, 1);
    while (1) {
        delay_ms(10);
    }
}
