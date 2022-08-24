#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <driver/uart.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include <string.h>
#include "PinNames.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "i2c.h"
// #include "mcpwm.h"
#include "ads1015.h"
#include "motor.h"
#include "pca9557.h"
#include "tof.h"
#include "twai.h"
#include "uart.h"
#include "commonfunc.h"

extern "C" {
void app_main(void);
}

TaskHandle_t taskHandle;

void IRAM_ATTR gpioIsr(void* arg) {
    BaseType_t taskWoken;
    // if(!(bool)gpio_get_level(*(gpio_num_t*)arg)){
    xTaskNotifyFromISR(taskHandle, 0, eNoAction, &taskWoken);
    // }
}

void isrTask(void* pvParameters) {
    // uint32_t ulNotifiedValue;
    while (1) {
        // xSemaphoreTake(semaphore,portMAX_DELAY);
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        if (!isPressed) {
            m1.write(0);
            printf("stop\n");
            isPressed = 1;
        } else {
            isPressed = 0;
        }
        // printf("changed\n");
    }
}

uart_isr_handle_t *handle_console;

void app_main() {
}
