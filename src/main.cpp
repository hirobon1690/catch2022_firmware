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

extern "C" {
void app_main(void);
}

gpio dir(E05, OUTPUT);
gpio slp(E04, OUTPUT);
gpio stp(P18, OUTPUT);
gpio pmp0(P18, OUTPUT);
gpio pmp1(P19, OUTPUT);
gpio vlv0(P27, OUTPUT);
gpio vlv1(P26,OUTPUT);

// Ticker ticker;
int stepCnt = 0;
int stepCycle = 0;

char prevbuf[128];
char buf[128];

// void IRAM_ATTR uartIsr(void *arg) {
//     int index = 0;
//     uart_read_bytes(UART_NUM_0, buf + index, 1, 10);
//     if (*(buf + index) == '\n') {
//         *(buf + index) = NULL;
//         memcpy(prevbuf,buf,sizeof(buf));
//         memset(buf, '\0', sizeof(buf));
//         return;
//     }
//     index++;
// }

void step() {
    if (stepCnt >= stepCycle) {
        stepCnt = 0;
    } else {
        stepCnt += 50;
    }
    stp.write((stepCycle != 0 && stepCnt < 100));
    if(stepCycle==0){
        slp.write(1);
    }else{
        slp.write(0);
    }
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();

    printf("OKOK\n");
    uart.init();
    // uart.enableIsr(uartIsr);

    // slp.write(1);
    // dir.write(1);
    // stp.write(0);
    // ticker.attach_us(50, step);
    // vlv0.write(1);
    // pmp0.write(1);
    // pmp1.write(1);
    printf("OK\n");
    // servo servo(P19, 0, 0);
    // int pastangle = 0;
    // printf("Enter Dir\n");
    // char direction[128];
    // uart.read(direction);
    // dir.write((bool)atoi(direction));
    
    while (1) {
        // stp.flip();
        delay_ms(10);
        char sample[128];
        printf("Enter Pomp State\n");
        uart.read(sample);
        switch(atoi(sample)){
            case 0:
                pmp0.write(1);
                vlv0.write(0);
                break;
            case 1:
                pmp0.write(1);
                vlv0.write(1);
                break;
            case 2:
                pmp1.write(1);
                vlv1.write(0);
                break;
            case 3:
                pmp1.write(1);
                vlv1.write(1);
                break;
            case 4:
                vlv0.write(0);
                vlv1.write(0);
                break;
            case 5:
                vlv0.write(1);
                vlv1.write(1);
                break;

        }
        // stepCycle = atoi(sample);
        // printf("\nCycle is %d\n", stepCycle);
        // servo.duty(angle);
        // delay_ms(10);
    }
}
