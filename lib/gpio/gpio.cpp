#pragma once
#include "gpio.h"
#include <driver/gpio.h>
#include "PinNames.h"
// #include "gpioex.h"
#include "pca9557.h"
#include "i2c.h"

gpio::gpio(PinName _pin, PinMode _mode) {
    pin = _pin;
    mode = _mode;
    if (pin < E00) {
        device = INTERNAL;
        switch (mode) {
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
        device = EXPANDER;
        ex.mode(pin, mode);
    }
}

gpio::gpio(){
    
}

void gpio::write(bool val) {
    state = val;
    switch (device) {
        case INTERNAL:
            gpio_set_level((gpio_num_t)pin, (uint32_t)val);
            break;
        case EXPANDER:
            ex.write(pin, val);
            break;
    }
}

bool gpio::read() {
    if (mode == OUTPUT) {
        return state;
    } else {
        switch (device) {
            case INTERNAL:
                return state = (bool)gpio_get_level((gpio_num_t)pin);
                break;
            case EXPANDER:
                return state = ex.read(pin);
                break;
            default:
                return state;
        }
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
