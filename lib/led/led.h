#pragma once
#include <driver/ledc.h>
#include "PinNames.h"

class RGBLED {
   public:
    RGBLED(PinName _red, PinName _green, PinName _blue);
    void writeRGB(int R, int G, int B);
    void writeHSV(int H, int S, int V);
    int rgb[3];
    int hsv[3];
};