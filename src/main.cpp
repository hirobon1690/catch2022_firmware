#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp32/rom/ets_sys.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/ets_sys.h>
#include <stdio.h>
#include "commonfunc.h"


extern "C" {
void app_main(void);
}

void app_main() {
    gpio_config_t conf = {
        .pin_bit_mask = (0b11ULL<<12),                 /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_DISABLE,                   /*!< GPIO mode: set input/output mode                     */
        .pull_up_en = GPIO_PULLUP_DISABLE,           /*!< GPIO pull-up                                         */
        .pull_down_en = GPIO_PULLDOWN_DISABLE,       /*!< GPIO pull-down                                       */
        // .intr_type = gpiohal.dev->pin[pin].int_type  /*!< GPIO interrupt type - previously set                 */
    };
    gpio_config(&conf);
    gpio_set_direction(GPIO_NUM_13,GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_13,1);
    gpio_set_level(GPIO_NUM_12,1);
    while(1){
        delay_ms(100);
    }

}
