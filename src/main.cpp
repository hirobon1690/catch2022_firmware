/* Uart Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "soc/uart_reg.h"
#include "soc/uart_struct.h"

#define BLINK_GPIO GPIO_NUM_2

static const char* TAG = "uart_events";

/**
 * This example shows how to use the UART driver to handle UART interrupt.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */

#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM (3) /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart0_queue;

// Both definition are same and valid
// static uart_isr_handle_t *handle_console;
static intr_handle_t handle_console;

// Receive buffer to collect incoming data
uint8_t rxbuf[256];
// Register to collect data length
uint16_t urxlen;

int counter=0;

#define NOTASK 0

extern "C" {
void app_main(void);
}

void blink_task(void* pvParameter) {
    vTaskDelay(1000);
}
/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
static void IRAM_ATTR uart_intr_handle(void* arg) {
    counter++;
    uint16_t rx_fifo_len, status;
    uint16_t i = 0;

    status = UART0.int_st.val;              // read UART interrupt Status
    rx_fifo_len = UART0.status.rxfifo_cnt;  // read number of bytes in UART buffer

    while (rx_fifo_len) {
        rxbuf[i++] = UART0.fifo.rw_byte;  // read all bytes
        rx_fifo_len--;
    }

    // after reading bytes from buffer clear UART interrupt status
    uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR | UART_RXFIFO_TOUT_INT_CLR);

    // a test code or debug code to indicate UART receives successfully,
    // you can redirect received byte as echo also
    uart_write_bytes(EX_UART_NUM, rxbuf, 2);
}
/*
 * main
 */
void app_main() {
    int ret;
    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    ESP_ERROR_CHECK(uart_param_config(EX_UART_NUM, &uart_config));

    // Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // Set UART pins (using UART0 default pins ie no changes.)
    ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Install UART driver, and get the queue.
    ESP_ERROR_CHECK(uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));

    // release the pre registered UART handler/subroutine
    ESP_ERROR_CHECK(uart_isr_free(EX_UART_NUM));

    // register new UART subroutine
    ESP_ERROR_CHECK(uart_isr_register(EX_UART_NUM, uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));

    // enable RX interrupt
    ESP_ERROR_CHECK(uart_enable_rx_intr(EX_UART_NUM));
    // #if (NOTASK == 1)
    while (1) {
        printf("%d\n",counter);
        vTaskDelay(100);
    }
    // #else
    // 	// xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    // #endif
}

// #include <driver/adc.h>
// #include <driver/gpio.h>
// #include <driver/i2c.h>
// #include <driver/mcpwm.h>
// #include <driver/uart.h>
// #include <esp32/rom/ets_sys.h>
// #include <rom/ets_sys.h>
// #include <stdio.h>
// #include <string.h>
// #include "PinNames.h"
// #include "ads1015.h"
// #include "driver/twai.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "gpio.h"
// #include "i2c.h"
// #include "pca9557.h"
// #include "tof.h"
// #include "twai.h"
// #include "uart.h"

// extern "C" {
// void app_main(void);
// }

// gpio dir0(E04, OUTPUT);
// gpio dir1(E01, OUTPUT);

// char prevbuf[128];
// char buf[128];
// int indexnum = 0;

// void IRAM_ATTR uartIsr(void* arg) {

//     // uart_read_bytes(UART_NUM_0, buf + index, 1, 1);
//     // if (*(buf + index) == '\n') {
//     //     *(buf + index) = NULL;
//     //     memcpy(prevbuf, buf, sizeof(buf));
//     //     memset(buf, '\0', sizeof(buf));
//     //     return;
//     // }
//     indexnum++;
// }

// void delay_ms(int ms) {
//     vTaskDelay(ms / portTICK_RATE_MS);
// }

// uart_isr_handle_t *handle_console;

// void app_main() {
//     delay_ms(10);
//     i2c.init();
//     ex.set();
//     uart.init();
//     // uart.enableIsr(uartIsr);
//     printf("init\n");
//     twai.init();

//     uart_isr_free(UART_NUM_0);
//     uart_isr_register(UART_NUM_0, uartIsr, NULL, ESP_INTR_FLAG_IRAM, handle_console);
//     uart_enable_rx_intr(UART_NUM_0);
//     unsigned char data[4] = {1, 2, 3, 4};
//     while (1) {
//         printf("%s", prevbuf);
//         delay_ms(10);
//     }

//     // while (1) {
//     // }
// }
