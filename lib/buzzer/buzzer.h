#pragma once
#include "gpio.h"
#include "commonfunc.h"

class buzzer:gpio{
    public:
    buzzer(PinName pin);
    void buzz(int times, int length=50);
};