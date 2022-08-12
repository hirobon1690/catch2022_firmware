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

extern "C" {
void app_main(void);
}

gpio dir0(E04, OUTPUT);
gpio dir1(E01, OUTPUT);

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void app_main() {
    delay_ms(10);
    i2c.init();
    ex.set();
    printf("init\n");

    // Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_23, GPIO_NUM_25, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }

    // Start TWAI driver
    if (twai_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
        return;
    }

    // Configure message to transmit
    twai_message_t message;
    message.identifier = 0xAAAA;
    message.extd = 1;
    message.data_length_code = 4;
    for (int i = 0; i < 4; i++) {
        message.data[i] = 0;
    }

    while (1) {
        // Queue message for transmission
        if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
            printf("Message queued for transmission\n");
        } else {
            printf("Failed to queue message for transmission\n");
        }
        delay_ms(1000);
    }

    // while (1) {
    // }
}
