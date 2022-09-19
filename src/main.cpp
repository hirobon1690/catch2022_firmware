// #define DEBUG

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
char uart_msg[20];
char uart_msg_tx[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0xFF};
float move_cmd[2];
float servo_angle;
int stepper_state;
int pmp_state;
int emergency;
int currentDeg[2] = {0, 0};
int preDeg[2] = {0, 0};
int newDeg[2] = {0, 0};
char is_grabbed = 0;
const int pidPeriod = 10;
int ledPeriod = 500;
struct {
    short hue = 0;
    unsigned char saturation = 0;
    unsigned char brightness = 0;
} led_hsv;
int result[2][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

motor m0(Pe1A, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, E04, E01);
motor m1(Pe1B, MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM1A, E02, E03);
gpio s00(Pe1C, INPUT_PU);
gpio s01(Pe1D, INPUT_PU);
gpio s10(Pe2A, INPUT_PU);
gpio s11(Pe2B, INPUT_PU);
gpio user(USER, INPUT_PU);
gpio pmp[2] = {gpio(Pe0A, OUTPUT), gpio(Pe0B, OUTPUT)};
gpio vlv[2] = {gpio(Pe0C, OUTPUT), gpio(Pe0D, OUTPUT)};
gpio userLed(LED, OUTPUT);
adc pot0(A1);
adc pot1(A0);
arm a0(m0, s00, s01, pot0, 250);
arm a1(m1, s10, s11, pot1, 276);
KRA_PID pid0((float)pidPeriod / 1000, 0, 250, 0, 35, 1.2);
KRA_PID pid1((float)pidPeriod / 1000, 0, 276, 0, 35, 1.2);

Ticker ticker0;
Ticker ticker1;

bool receiveFlag = 0;

float unpackFloat(char* buf, int index) {
    return *(float*)(buf + index);
}

int unpackInt(char* buf, int index) {
    return *(buf + index);
}

short unpackShort(char* buf, int index) {
    return *(short*)(buf + index);
}

void turnPmp(int val) {
    pmp[0].write(val & 1);
    vlv[0].write(!(val & 1));
    pmp[1].write(val >> 1);
    vlv[1].write(!(val >> 1));
}

void receiveUart(void* pvParameters) {
    while (1) {
        *(float*)uart_msg_tx = a0.calDeg(currentDeg[0]);
        // *(float*)uart_msg_tx = currentDeg[0];
        *(float*)(uart_msg_tx + 4) = a1.calDeg(currentDeg[1]);
        // *(float*)(uart_msg_tx + 4) = currentDeg[1];
        uart_msg_tx[8] = is_grabbed;
        uart_write_bytes(UART_NUM_0, uart_msg_tx, 11);
        // uart_read_bytes(UART_NUM_0, uart_msg, 16, portMAX_DELAY);
        uart_read_bytes(UART_NUM_0, uart_msg, 20, portMAX_DELAY);
        if (uart_msg[19] != 0xFF) {
            printf("Connection Failed!\n");
            while (1) {
                char buf[1];
                uart_read_bytes(UART_NUM_0, buf, 1, portMAX_DELAY);
                if (buf[0] == 0xFF) {
                    break;
                }
            }
        }
        // printf("%d\n",is_grabbed);
        move_cmd[0] = unpackFloat(uart_msg, 0);
        if (move_cmd[0] < 0) {
            move_cmd[0] = 0;
        } else if (move_cmd[0] > 250) {
            move_cmd[0] = 250;
        }
        move_cmd[1] = unpackFloat(uart_msg, 4);
        if (move_cmd[0] < 0) {
            move_cmd[0] = 0;
        } else if (move_cmd[0] > 276) {
            move_cmd[0] = 276;
        }
        pid0.setgoal(move_cmd[0]);
        pid1.setgoal(move_cmd[1]);
        servo_angle = unpackFloat(uart_msg, 8);
        stepper_state = unpackInt(uart_msg, 12);
        pmp_state = unpackInt(uart_msg, 13);
        turnPmp(pmp_state);
        emergency = unpackInt(uart_msg, 14);
        led_hsv.hue = unpackShort(uart_msg, 15);
        led_hsv.saturation = (unsigned char)uart_msg[17];
        led_hsv.brightness = (unsigned char)uart_msg[18];
        delay_ms(1);
    }
}

void sendTwai(void* pvParameters) {
    while (1) {
        unsigned char twai_msg_tx[8];
        *(float*)(twai_msg_tx) = servo_angle;
        twai_msg_tx[4] = stepper_state;
        unsigned char hsv[4];
        *(short*)(hsv) = led_hsv.hue;
        hsv[2] = led_hsv.saturation;
        hsv[3] = led_hsv.brightness;
        twai_msg_tx[5] = hsv[0];
        twai_msg_tx[6] = hsv[1];
        twai_msg_tx[7] = hsv[2];
        // twai_msg_tx[8] = hsv[3];
        twai.write(0x00, twai_msg_tx, 8);
        delay_ms(5);
    }
}

void receiveTwai(void* pvParameters) {
    while (1) {
        twai_message_t msg;
        twai.read(&msg);
        is_grabbed = msg.data[0];
        // printf("%d\n",is_grabbed);
        delay_ms(1);
    }
}

void flip(void* pvParameters) {
    userLed.write(1);
    while (1) {
        if (ledPeriod == 0) {
            userLed.write(1);
            delay_ms(10);
        } else {
            userLed.flip();
            delay_ms(ledPeriod);
        }
    }
}

void calPID() {
    newDeg[0] = pot0.read();
    newDeg[1] = pot1.read();
    for (int i = 0; i < 2; i++) {
        if (abs(preDeg[i] - newDeg[i]) < 60) {
            currentDeg[i] = newDeg[i];
            // a = newDeg[i];
        }
    }
#ifdef DEBUG
    printf("%3d, %3d, %d, %d, %f, %f\n", a0.calDeg(currentDeg[0]), a1.calDeg(currentDeg[1]), currentDeg[0], currentDeg[1], m0.duty, m1.duty);
#endif
    int duty[2] = {0, 0};
    duty[0] = (int)pid0.calPID(a0.calDeg(currentDeg[0]));
    duty[1] = (int)pid1.calPID(a1.calDeg(currentDeg[1]));
    if (pid0.judgePID()) {
        m0.write(0);
    } else {
        m0.write(duty[0]);
    }
    if (pid1.judgePID()) {
        m1.write(0);
    } else {
        m1.write(duty[1]);
    }
    preDeg[0] = currentDeg[0];
    preDeg[1] = currentDeg[1];
}

int rawData[2] = {0, 0};

void app_main() {
    // disableCore0WDT();
    // disableCore1WDT();
    init();
    turnPmp(0);
    m0.write(0);
    m1.write(0);
    xTaskCreatePinnedToCore(flip, "flip", 1024, NULL, 21, &taskHandle, 1);

#ifdef DEBUG
    while (1) {
        int a = pot0.read();
        int b = pot1.read();
        printf("%d, %d\n", a, b);
        if (!user.read()) {
            break;
        }
        delay_ms(5);
        preDeg[0] = a;
        preDeg[1] = b;
    }
#else
    char start_msg[4];
    ledPeriod = 500;
    while (1) {
        int a = pot0.read();
        int b = pot1.read();
        uart_read_bytes(UART_NUM_0, start_msg, 4, portMAX_DELAY);
        if (start_msg[0] == 0xFF && start_msg[1] == 0xFF && start_msg[2] == 0xFF && start_msg[3] == 0xFF) {
            uart_write_bytes(UART_NUM_0, start_msg, 4);
            break;
        }
    }
    unsigned char twai_msg_tx[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    twai.write(0x00, twai_msg_tx, 8);
    ledPeriod = 200;
    for (int i = 0; i < 4; i++) {
        start_msg[i] = 0;
    }
    while (1) {
        int a = pot0.read();
        int b = pot1.read();
        uart_read_bytes(UART_NUM_0, start_msg, 4, portMAX_DELAY);
        if (start_msg[0] == 0xFF && start_msg[1] == 0xFF && start_msg[2] == 0xFF && start_msg[3] == 0xFF) {
            uart_write_bytes(UART_NUM_0, start_msg, 4);
            break;
        }
    }
#endif
    ledPeriod = 0;
    currentDeg[0] = pot0.read();
    currentDeg[1] = pot1.read();
    // preDeg[0] = currentDeg[0];
    // preDeg[1] = currentDeg[1];
    pid0.setgain(7, 0, 0);
    pid1.setgain(6, 0, 0);
    a0.home(0, 1433, 99);
    a1.home(0, 214, 1413);
    pid0.setgoal(a0.calDeg(currentDeg[0]));
    pid1.setgoal(a1.calDeg(currentDeg[1]));
    ticker0.attach_ms(pidPeriod, calPID);
    xTaskCreatePinnedToCore(sendTwai, "sendTwai", 2048, NULL, 21, &taskHandle, 0);
#ifndef DEBUG
    xTaskCreatePinnedToCore(receiveUart, "receiveUart", 4096, NULL, 22, &taskHandle, 0);
#endif
    xTaskCreatePinnedToCore(receiveTwai, "receiveTwai", 4096, NULL, 23, &taskHandle, 0);
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
        delay_ms(1);
    }
}
