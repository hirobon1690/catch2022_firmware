#pragma once
#include "i2c.h"

enum pins{
    A0=0b100,
    A1=0b101,
    A2=0b110,
    A3=0b111
};
class adc{
    public:
        adc(pins);
        void init();
        int read();
        int readAvrg(int times);

    private:
        int pin;
        uint8_t conf[3]={0b00000001,0b10000010,0b10100011};
        const uint8_t ADDR=0x48;
};
