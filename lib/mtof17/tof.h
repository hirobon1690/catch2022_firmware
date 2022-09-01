#pragma once
#include "i2c.h"
#include "gpio.h"
#include "PinNames.h"
#include "commonfunc.h"

class tof{
    public:
    tof(gpio&);
    int read();

    private:
    const int ADDR=0x52;
    PinName devicePin;
    gpio& select;
};