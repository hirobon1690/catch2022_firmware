#include "gpioex.h"
#include "PinNames.h"
#include "i2c.h"

gpioex ex;

void gpioex::mode(PinName _pin, PinMode mode) {
    // modeが0で出力，1で入力，pullが0でプルダウン，1でプルアップ
    int pin = (int)_pin - 40;
    switch (mode) {
        case INPUT_PU:
            config = config | (1 << pin);
            pullState = pullState | (1 << pin);
            break;

        case INPUT_PD:
            config = config | (1 << pin);
            pullState = pullState | (0 << pin);
            break;

        default:
            config = config | (0 << pin);
            pullState = pullState | (0 << pin);
    }
}

void gpioex::set() {
    uint8_t cfg[3] = {0x07, uint8_t(config >> 8), uint8_t(config)};
    i2c.write(ADDR, cfg, 3);
    uint8_t pst[3] = {0x49, uint8_t(pullState >> 8), uint8_t(pullState)};
    i2c.write(ADDR, pst, 3);
}

void gpioex::write(PinName _pin, bool val) {
    int pin = (int)_pin - 40;
    if (val) {
        output |= (val << pin);
    } else {
        output &= ~(!val << pin);
    }
    uint8_t command[3] = {0x03, uint8_t(output >> 8), uint8_t(output)};
    i2c.write(ADDR, command, 3);
}

bool gpioex::read(PinName _pin) {
    int pin = (int)_pin - 40;
    uint8_t result[2];
    i2c.write(ADDR, 0x01);
    i2c.read(ADDR, result, 2);
    input = result[0] << 8 | result[1];
    return (input >> pin) & 0x0001;
}