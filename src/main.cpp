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

motor m0(Pe1C,MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM0A,E04,E01);
motor m1(Pe1D,MCPWM_UNIT_1,MCPWM_TIMER_1,MCPWM1A,E02,E03);
gpio s00(Pe1A,INPUT_PU);
gpio s01(Pe1B,INPUT_PU);
gpio s10(Pe2A,INPUT_PU);
gpio s11(Pe2B,INPUT_PU);
gpio user(USER,INPUT_PU);
adc pot0(A2);
adc pot1(A3);
arm a0(m0,user,user,pot0,250);
arm a1(m1,user,user,pot1,276);
KRA_PID pid0(1,0,250,0,30);
KRA_PID pid1(1,0,276,0,30);

Ticker ticker0;
Ticker ticker1;

float unpackFloat(char* buf, int index) {
    return *(float*)(buf + index);
}

int unpackInt(char* buf, int index) {
    return *(buf + index);
}

void receiveUart(void* pvParameters) {
    while (1) {
        uart_read_bytes(UART_NUM_0, uart_msg, 16, portMAX_DELAY);
        if (uart_msg[15] != 0xFF) {
            printf("Connection Failed!\n");
        }
        move_cmd[0] = unpackFloat(uart_msg, 0);
        move_cmd[1] = unpackFloat(uart_msg, 4);
        servo_angle = unpackFloat(uart_msg, 8);
        stepper_state = unpackInt(uart_msg, 12);
        pump_state = unpackInt(uart_msg, 13);
        emergency = unpackInt(uart_msg, 14);
    }
}

void calA0PID(){
    m0.write(pid0.calPID(a0.getDeg()));
}

void calA1PID(){
    m1.write(pid1.calPID(a1.getDeg()));
}

void app_main() {
    i2c.init();
    ex.set();
    uart.init();
    twai.init();
    pid0.setgain(10,0,0);
    pid1.setgain(10,1,1);
    ticker0.attach_ms(1,calA0PID);
    ticker1.attach_ms(1,calA1PID);
    a0.home(30);
    xTaskCreatePinnedToCore(receiveUart, "receiveUart", 4096, NULL, 22, &taskHandle, 0);
    while (1) {
        delay_ms(10);
    }
}
