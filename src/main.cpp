/* Uart Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"

static const char *TAG = "uart_events";

/**
 * This example shows how to use the UART driver to handle special UART events.
 *
 * It also reads data from UART0 directly, and echoes it to console.
 *
 * - port: UART0
 * - rx buffer: on
 * - tx buffer: on
 * - flow control: off
 * - event queue: on
 * - pin assignment: txd(default), rxd(default)
 */

#define EX_UART_NUM UART_NUM_0

#define BUF_SIZE (1024)
static QueueHandle_t uart0_queue;

extern "C" {
void app_main(void);
}

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(BUF_SIZE);
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.
                in this example, we don't process data in event, but read data outside.*/
                case UART_DATA:
                    uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                    ESP_LOGI(TAG, "data, len: %d; buffered len: %d", event.size, buffered_size);
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow\n");
                    //If fifo overflow happened, you should consider adding flow control for your application.
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(EX_UART_NUM);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full\n");
                    //If buffer full happened, you should consider encreasing your buffer size
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(EX_UART_NUM);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break\n");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error\n");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error\n");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    ESP_LOGI(TAG, "uart pattern detected\n");
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d\n", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

/* Configure the UART events example */
void app_main()
{
    uart_config_t uart_config = {
       .baud_rate = 115200,
       .data_bits = UART_DATA_8_BITS,
       .parity = UART_PARITY_DISABLE,
       .stop_bits = UART_STOP_BITS_1,
       .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
       .rx_flow_ctrl_thresh = 122,
    };
    //Set UART parameters
    uart_param_config(EX_UART_NUM, &uart_config);
    //Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    //Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart0_queue, 0);

    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Set uart pattern detect function.
    uart_enable_pattern_det_intr(EX_UART_NUM, '+', 3, 10000, 10, 10);
    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
    //process data
    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    do {
        int len = uart_read_bytes(EX_UART_NUM, data, BUF_SIZE, 100 / portTICK_RATE_MS);
        if(len > 0) {
            ESP_LOGI(TAG, "uart read : %d", len);
            uart_write_bytes(EX_UART_NUM, (const char*)data, len);
        }
    } while(1);
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
