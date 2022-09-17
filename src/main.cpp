#include <driver/adc.h>
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
#include "pca9557.h"
#include "ads1015.h"

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);

void delay_ms(int ms) {
    vTaskDelay(ms/portTICK_RATE_MS);
}



void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    adc pin0(A0);
    adc pin1(A1);
    int result[2]={0,0};

    printf("init\n");
    while (1) {
        // result[0]=pin0.read();
            result[0]=pin0.read();
            delay_ms(1);    
            result[1]=pin1.read();
            delay_ms(1);    
        
        // result[1]=pin1.read();
        printf("% 4d, % 4d\n",result[0],result[1]);
        
    }
}
