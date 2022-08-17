#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <driver/uart.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include <string.h>
#include "PinNames.h"
#include "Ticker.h"
#include "ads1015.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "i2c.h"
#include "pca9557.h"
#include "servo.h"
#include "stepper.h"
#include "tof.h"
#include "uart.h"

extern "C" {
void app_main(void);
}

gpio dir(E05, OUTPUT);
gpio slp(E04, OUTPUT);
gpio stp(P18, OUTPUT);
gpio pmp0(E06, OUTPUT);
gpio pmp1(E01, OUTPUT);
gpio vlv0(E02, OUTPUT);

Ticker ticker;
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

    slp.write(1);
    dir.write(1);
    stp.write(0);
    ticker.attach_us(50, step);
    vlv0.write(1);
    pmp0.write(1);
    pmp1.write(1);
    printf("OK\n");
    servo servo(P19, 0, 0);
    int pastangle = 0;
    printf("Enter Dir\n");
    char direction[128];
    uart.read(direction);
    dir.write((bool)atoi(direction));
    
    while (1) {
        // stp.flip();
        delay_ms(10);
        char sample[128];
        printf("Enter Stepcycle\n");
        uart.read(sample);
        stepCycle = atoi(sample);
        printf("\nCycle is %d\n", stepCycle);
        // servo.duty(angle);
        // delay_ms(10);
    }
}
