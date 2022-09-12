#pragma once
#include <driver/gpio.h>
#include "PinNames.h"

class gpio {
   public:
    gpio(PinName _pin, PinMode _mode);
    gpio();
    void write(bool val);
    bool read();
    void flip();
    gpio& operator=(bool val);
    operator bool();
    operator int();
    PinName pin;

   private:
    PinMode mode;
    Device device;
    bool state = 0;
};
