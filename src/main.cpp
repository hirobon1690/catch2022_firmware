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
    uint8_t out[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    i2c.init();
    i2c.write(0x20, data1, sizeof(data1));
    while (1) {
        i2c.write(0x20, data1, 1);
        i2c.read(0x20, out, 8);
        delay_ms(500);
        printf("%d, %d, %d, %d, %d, %d, %d, %d\n", out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
    }
}
