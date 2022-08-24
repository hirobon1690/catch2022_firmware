#include "commonfunc.h"

void enableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void disableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void delay_ms(int ms) {
    vTaskDelay(ms/portTICK_RATE_MS);
}