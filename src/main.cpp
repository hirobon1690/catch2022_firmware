#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
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

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);
gpio user(P2, INPUT_PU);
gpio limit0(Pe2A,INPUT_PU);
gpio limit1(Pe2B,INPUT_PU);

TaskHandle_t taskHandle;
volatile SemaphoreHandle_t semaphore;

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

int i = 0;
void IRAM_ATTR gpioIsr(void* arg) {
    i++;
    BaseType_t taskWoken;
    xTaskNotifyFromISR(taskHandle,0,eNoAction,&taskWoken);
    // xSemaphoreGiveFromISR(semaphore,NULL);
}

void isrTask(void* pvParameters) {
    // uint32_t ulNotifiedValue;
    while (1) {
        // xSemaphoreTake(semaphore,portMAX_DELAY);
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        printf("changed\n");
        delay_ms(100);
    }
}

void app_main() {
    i2c.init();
    ex.set();
    gpio_install_isr_service(0);
    gpio_set_intr_type((gpio_num_t)Pe2B, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add((gpio_num_t)Pe2B, gpioIsr, NULL);
    semaphore=xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(isrTask, "isrTask", 4096, NULL, 10, &taskHandle, 0);

    while (1) {
        // printf("%d\n", i);
        // if(!user.read()){
        //     printf("1\n");
        // }else{
        //     printf("0\n");
        // }
        // xTaskNotify(taskHandle,0,eNoAction);
        delay_ms(100);
    }
}