#pragma once
#include <driver/gpio.h>
#include "PinNames.h"

class gpio{
    public:
        gpio(PinName _pin,PinMode _mode);
        void write(bool val);
        bool read();
        void flip();
        gpio& operator= (bool val);
        operator bool();
        operator int();
    
    private:
        PinName pin;
        PinMode mode;
        bool state=0;
};