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
char uart_msg_tx[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0xFF};
float move_cmd[2];
float servo_angle;
int stepper_state;
int pmp_state;
int emergency;
int currentDeg[2] = {0, 0};
int preDeg[2] = {0, 0};
int newDeg[2] = {0, 0};
bool is_grabbed = 0;
const int pidPeriod = 10;

motor m0(Pe1A, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, E01, E04);
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
KRA_PID pid0((float)pidPeriod / 1000, 0, 250, 0, 15, 0.4);
KRA_PID pid1((float)pidPeriod / 1000, 0, 276, 0, 15, 0.4);

Ticker ticker0;
Ticker ticker1;

float unpackFloat(char* buf, int index) {
    return *(float*)(buf + index);
}

int unpackInt(char* buf, int index) {
    return *(buf + index);
}

void turnPmp(bool val) {
    for (int i = 0; i < 1; i++) {
        pmp[i].write(val);
        vlv[i].write(val);
    }
}

void receiveUart(void* pvParameters) {
    while (1) {
        *(float*)uart_msg_tx = a0.calDeg(currentDeg[0]);
        *(float*)(uart_msg_tx + 4) = a1.calDeg(currentDeg[1]);
        *(bool*)(uart_msg_tx + 9) = is_grabbed;
        uart_write_bytes(UART_NUM_0, uart_msg_tx, 11);
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
        // turnPmp(stepper_state);
        pmp_state = (bool)unpackInt(uart_msg, 13);
        turnPmp(pmp_state);
        emergency = unpackInt(uart_msg, 14);
    }
}

void sendTwai(void* pvParameters) {
    while (1) {
        unsigned char twai_msg_tx[5];
        *(float*)(twai_msg_tx) = servo_angle;
        twai_msg_tx[4]=stepper_state;
        // printf("%d\n",stepper_state);
        twai.write(0x00,twai_msg_tx,5);
        delay_ms(10);
    }
}

void calPID() {
    // printf("%3.2f, %3.2f, %d, %d, %f, %f\n", a0.calDeg(currentDeg[0]), a1.calDeg(currentDeg[1]), currentDeg[0], currentDeg[1], m0.duty, m1.duty);
    // xTaskNotify(taskHandle, 0, eNoAction);
    // float currentDeg[2] = {0, 0};
    // while (1) {
    // currentDeg[0] = a0.getDeg();
    // delay_ms(2);
    // currentDeg[1] = a1.getDeg();
    // delay_ms(2);
    // m0.write(pid0.calPID(currentDeg[0]));
    m0.write(pid0.calPID(a0.calDeg(currentDeg[0])));
    m1.write(pid1.calPID(a1.calDeg(currentDeg[1])));
    // printf("%f, %f\n", currentDeg[0], currentDeg[1]);
    // delay_ms(8);
    // }
}

void adctest(void* pvParameters) {
    // int currentDeg[2] = {0, 0};
    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        // currentDeg[0] = pot0.read();  // a0.getDeg();
        // currentDeg[1] = pot1.read();  // a1.getDeg();
        // delay_ms(3);
        // m0.write(pid0.calPID(currentDeg[0]));
        // m1.write(pid1.calPID(currentDeg[1]));
        // printf("%3.2f, %3.2f, %d, %d\n", a0.calDeg(currentDeg[0]), a1.calDeg(currentDeg[1]), currentDeg[0], currentDeg[1]);
    }
}

void adConvert(void* pvParameters) {
    while (1) {
        newDeg[0] = pot0.readAvrg(10);
        delay_ms(5);
        newDeg[1] = pot1.readAvrg(10);
        delay_ms(5);
        if (abs(currentDeg[0] - newDeg[0]) < 50) {
            currentDeg[0] = newDeg[0];
        }
        if (abs(currentDeg[1] - newDeg[1]) < 50) {
            currentDeg[1] = newDeg[1];
        }
        // printf("%d, %d\n", currentDeg[0], currentDeg[1]);
        // if (abs(currentDeg[0] - preDeg[0]) >= 100) {
        //     currentDeg[0] = preDeg[0];
        //     continue;
        // }
        // if (abs(currentDeg[1] - preDeg[1]) >= 100) {
        //     currentDeg[1] = preDeg[1];
        //     continue;
        // }
        // preDeg[0]=currentDeg[0];
        // preDeg[1]=currentDeg[1];
        delay_ms(5);
    }
}

int rawData[2] = {0, 0};

void app_main() {
    init();
    int result[2];
    currentDeg[0] = pot0.readAvrg(10);
    delay_ms(5);
    currentDeg[1] = pot1.readAvrg(10);
    delay_ms(5);
    // printf("init\nPress USER to start\n");
    while (1) {
        if (!user.read()) {
            break;
        }
        delay_ms(10);
    }
    pid0.setgain(10, 0, 0);
    pid1.setgain(10, 0, 0);
    // m0.write(-10);
    // m1.write(10);
    // while (1) {
    //     for (int i = 0; i < 50; i++) {
    //         // m0.write(i);
    //         m1.write(i);
    //         delay_ms(100);
    //     }
    //     for (int i = 50; i > 0; i--) {
    //         // m0.write(i);
    //         m1.write(i);
    //         delay_ms(100);
    //     }
    // }

    pot0.readAvrg(100);
    pot1.readAvrg(100);
    // a0.home(15);
    a0.home(0, 900, 94);
    // a1.home(30);
    a1.home(0, 140, 1040);
    // a0.home(15);
    // a1home();
    pid0.setgoal(125);
    pid1.setgoal(138);
    ticker0.attach_ms(pidPeriod, calPID);

    // ticker1.attach_ms(pidPeriod,calA1PID);
    xTaskCreatePinnedToCore(sendTwai, "sendTwai", 2048, NULL, 21, &taskHandle, 0);
    xTaskCreatePinnedToCore(receiveUart, "receiveUart", 4096, NULL, 22, &taskHandle, 0);
    // xTaskCreatePinnedToCore(adctest, "adctest", 4096, NULL, 23, &taskHandle, 1);
    xTaskCreatePinnedToCore(adConvert, "adConvert", 2048, NULL, 22, &taskHandle, 1);
    while (1) {
        if (!s00.read() && m0.duty > 0) {
            m0.write(0);
        }
        if (!s01.read() && m0.duty < 0) {
            m0.write(0);
        }
        if (!s10.read() && m1.duty > 0) {
            m1.write(0);
        }
        if (!s11.read() && m1.duty < 0) {
            m1.write(0);
        }
        delay_ms(10);
    }
}
