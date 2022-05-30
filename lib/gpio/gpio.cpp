#include "gpio.h"
#include <driver/gpio.h>
#include "PinNames.h"
#include "i2c.h"

gpio::gpio(PinName _pin, PinMode _mode) {
    pin = _pin;
    mode = _mode;
    if (pin < E00) {
        switch (_mode) {
            case INPUT_PU:
                gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
                gpio_set_pull_mode((gpio_num_t)pin, GPIO_PULLUP_ONLY);
                break;
            case INPUT_PD:
                gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
                gpio_set_pull_mode((gpio_num_t)pin, GPIO_PULLDOWN_ONLY);
                break;
            default:
                gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT);
                break;
        }
    } else {
    }
}

void gpio::write(bool val) {
    gpio_set_level((gpio_num_t)pin, (uint32_t)val);
    state = val;
}

bool gpio::read() {
    if (mode == OUTPUT) {
        return state;
    } else {
        return state = (bool)gpio_get_level((gpio_num_t)pin);
    }
}

void gpio::flip() {
    write(!state);
}

gpio& gpio::operator=(bool val) {
    write(val);
    return *this;
}

gpio::operator bool() {
    return read();
}

gpio::operator int() {
    return read();
}
