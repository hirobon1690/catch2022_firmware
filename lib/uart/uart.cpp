#include "uart.h"
#include <driver/gpio.h>
#include <stdio.h>
#include <string.h>

_uart uart;

void _uart::init() {
    uart_config_t uart_config;
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void _uart::write(char* val) {
    uart_write_bytes(UART_NUM_0, (const char*)val, strlen(val));
}

void _uart::read(char* val) {
    int index = 0;
    while (1) {
        uart_read_bytes(UART_NUM_0, val + index, 1, 1000000);
        if (*(val+index) == '\n') {
            *(val+index) =NULL;
            return;
        }
        index++;
    }
}