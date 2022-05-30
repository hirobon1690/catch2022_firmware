#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp32/rom/ets_sys.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"

extern "C" {
void app_main(void);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    i2c i2c;
    // uint8_t data1[]={0x06,0x00,0x00};
    uint8_t data1[] = {0x00};
    uint8_t data[] = {0x02, 0x00, 0x00};
    uint8_t data3[] = {0x02, 0xFF, 0xFF};
    uint8_t out[2] = {0, 0};
    i2c.init();
    i2c.write(0x20, data1, sizeof(data1));
    while (1) {
        i2c.write(0x20, data1, 1);
        i2c.read(0x20, out, 2);
        uint16_t out1=out[1]<<8|out[0];
        bool port[16]={0};
        for(int i=0;i<16;i++){
            port[i]=(out1>>i)&0b0000000000000001;
        }
        delay_ms(500);
        printf("%d, %d, %d, %d, %d, %d, %d, %d\n", port[0],port[1],port[2],port[3],port[4],port[5],port[6],port[7]);
        printf("%d, %d, %d, %d, %d, %d, %d, %d\n\n", port[8],port[9],port[10],port[11],port[12],port[13],port[14],port[15]);
    }
}
