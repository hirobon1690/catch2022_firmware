#pragma once
#include <driver/ledc.h>
#include "PinNames.h"

class servo{
    public:
    servo(PinName pin, int min, int max);
    void write(float angle);
    void duty(uint32_t duty);

    private:
    const int max=508;
    const int min=104;
    float angle;

};