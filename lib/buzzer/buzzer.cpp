#include "buzzer.h"

buzzer::buzzer(PinName pin)
    : gpio(pin, OUTPUT) {
}

void buzzer::buzz(int times, int length) {
    for (int i = 0; i < times; i++) {
        this->write(1);
        delay_ms(length);
        this->write(0);
        delay_ms(30);
    }
}