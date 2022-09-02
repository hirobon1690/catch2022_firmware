#include "commonfunc.h"

void enableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void disableCore0WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
}

void disableCore1WDT() {
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(1);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

void init() {
    i2c.init();
    twai.init();
    ex.set();
    uart.init();
    gpio_config_t conf = {
        .pin_bit_mask = (0b11ULL << 12),       /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_OUTPUT,             /*!< GPIO mode: set input/output mode                     */
        .pull_up_en = GPIO_PULLUP_DISABLE,     /*!< GPIO pull-up                                         */
        .pull_down_en = GPIO_PULLDOWN_DISABLE, /*!< GPIO pull-down                                       */
        // .intr_type = gpiohal.dev->pin[pin].int_type  /*!< GPIO interrupt type - previously set                 */
    };
    gpio_config(&conf);
}