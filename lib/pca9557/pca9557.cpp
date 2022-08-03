#include "pca9557.h"
#include "PinNames.h"
#include "i2c.h"

gpioex ex;

void gpioex::mode(PinName _pin, PinMode mode) {
    // modeが0で出力，1で入力，pullが0でプルダウン，1でプルアップ
    int pin = (int)_pin - 40;
    switch (mode) {
        case INPUT:
            config = config | (1 << pin);
            break;

        default:
            config = config | (0 << pin);
    }
}

void gpioex::set() {
    uint8_t cfg[2] = {0x03, uint8_t(config)};
    i2c.write(ADDR, cfg, 2);
    // uint8_t pst[3] = {0x49, uint8_t(pullState >> 8), uint8_t(pullState)};
    // i2c.write(ADDR, pst, 3);
}

void gpioex::write(PinName _pin, bool val) {
    int pin = (int)_pin - 40;
    if (val) {
        output |= (val << pin);
    } else {
        output &= ~(!val << pin);
    }
    uint8_t command[2] = {0x01,output};
    i2c.write(ADDR, command, 2);
}

bool gpioex::read(PinName _pin) {
    int pin = (int)_pin - 40;
    uint8_t result[1];
    i2c.write(ADDR, 0x00);
    i2c.read(ADDR, result, 1);
    input = result[0];
    return (input >> pin) & 0x0001;
}