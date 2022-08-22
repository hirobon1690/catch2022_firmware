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
adc pot0(A2);

// mcpwm pwm0(P14, MCPWM_UNIT_0, MCPWM0A);
motor m0(Pe1A, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, E04, E01);
motor m1(Pe1B, MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM1A, E03, E02);

bool isPressed = 0;

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
        if (!isPressed) {
            m0.write(0);
            printf("stop\n");
            isPressed = 1;
        } else {
            isPressed = 0;
        }
        // printf("changed\n");
    }
}

int rawData[2]={0,0};

void home(motor &_motor){
    _motor.write(15);
    while(1){
        if(!s1.read()){
            break;
        }
        delay_ms(1);
    }
    _motor.write(0);
    rawData[0]=pot0.readAvrg(100);
    printf("%d\n",rawData[0]);
    _motor.write(-15);
    while(1){
        if(!s0.read()){
            break;
        }
        delay_ms(1);
    }
    _motor.write(0);
    rawData[1]=pot0.readAvrg(100);
    printf("%d\n",rawData[1]);
}

void app_main() {
    Ticker ticker;
    delay_ms(10);
    i2c.init();
    ex.set();
    // disableCore0WDT();
    uart.init();
    home(m1);

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

    while (1) {
        // printf("%d\n",a0.read());
        float result=0;
        for(int i=0;i<100;i++){
            result+=a0.read();
        }
        result/=100;
        result=-6E-06*result*result-0.1835*result+267.85;
        m0.write(pid.calPID(result));
        delay_ms(10);

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
