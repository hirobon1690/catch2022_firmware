#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/mcpwm.h>
#include <esp32/rom/ets_sys.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include "PinNames.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "i2c.h"
#include "Ticker.h"
// #include "mcpwm.h"
#include "ads1015.h"
#include "motor.h"
#include "pca9557.h"
#include "uart.h"
#include "KRA_PID.h"

extern "C" {
void app_main(void);
}

// gpio dir0(E04, OUTPUT);
// gpio dir1(E01, OUTPUT);
gpio dir2(E02, OUTPUT);
gpio dir3(E03, OUTPUT);

KRA_PID pid(0.01,0,250,0,30);

gpio s0(Pe1C, INPUT_PU);
gpio s1(Pe1D, INPUT_PU);
gpio user(USER, INPUT_PU);

// mcpwm pwm0(P14, MCPWM_UNIT_0, MCPWM0A);
motor m0(Pe1A, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, E04, E01);
motor m1(Pe1B, MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM1A, E03, E02);

void enableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void disableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
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
        m0.write(0);
        // printf("changed\n");
    }
}

void app_main() {
    Ticker ticker;
    delay_ms(10);
    i2c.init();
    ex.set();
    // disableCore0WDT();
    uart.init();

    gpio_install_isr_service(0);
    gpio_set_intr_type((gpio_num_t)Pe1C, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type((gpio_num_t)Pe1D, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type((gpio_num_t)USER, GPIO_INTR_NEGEDGE);
    gpio_isr_handler_add((gpio_num_t)Pe1C, gpioIsr, NULL);
    gpio_isr_handler_add((gpio_num_t)Pe1D, gpioIsr, NULL);
    gpio_isr_handler_add((gpio_num_t)USER, gpioIsr, NULL);
    xTaskCreatePinnedToCore(isrTask, "isrTask", 4096, NULL, 10, &taskHandle, 0);
    pid.setgain(10,1,0);
    pid.setgoal(125);

    printf("init\n");
    int duty = 0;
    adc a0(A2);
    while (1) {
        printf("%d\n",a0.read());
        int result=0;
        for(int i=0;i<100;i++){
            result+=a0.read();
        }
        result/=100;
        
        delay_ms(100);

        // delay_ms(10);
        // char sample[128];
        // printf("Enter duty\n");
        // uart.read(sample);
        // if (sample[0] == 'r') {
        //     int result = 0;
        //     for (int i = 0; i < 100; i++) {
        //         result += a0.read();
        //         delay_ms(1);
        //     }
        //     printf("%d\n", result / 100);
        // } else {
        //     duty = atoi(sample);
        //     m0.write(duty);
        //     printf("Duty is %d\n", duty);
        // }

    }
}
