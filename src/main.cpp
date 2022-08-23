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
#include "ads1015.h"
#include "driver/twai.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "i2c.h"
#include "pca9557.h"
#include "tof.h"
#include "twai.h"
#include "uart.h"

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);

char prevbuf[128];
char buf[128];
int indexnum = 0;

void IRAM_ATTR uartIsr(void* arg) {

    // uart_read_bytes(UART_NUM_0, buf + index, 1, 1);
    // if (*(buf + index) == '\n') {
    //     *(buf + index) = NULL;
    //     memcpy(prevbuf, buf, sizeof(buf));
    //     memset(buf, '\0', sizeof(buf));
    //     return;
    // }
    indexnum++;
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

uart_isr_handle_t *handle_console;

void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    uart.init();
    // uart.enableIsr(uartIsr);
    printf("init\n");
    twai.init();

    // uart_isr_free(UART_NUM_0);
    // uart_isr_register(UART_NUM_0, uartIsr, NULL, ESP_INTR_FLAG_IRAM, handle_console);
    // uart_enable_rx_intr(UART_NUM_0);
    char data[8] = {0,0,0,0,0,0,0,0};
    float unpack[2]={0,0};
    while (1) {
        uart_read_bytes(UART_NUM_0, data, 8, portMAX_DELAY);
        unpack[0]=*(float*)data;
        unpack[1]=*(float*)(data+4);
        printf("%f, %f\n", unpack[0],unpack[1]);
        delay_ms(10);
    }

    // while (1) {
    // }
}
