#pragma once
#include <driver/ledc.h>
#include "PinNames.h"

class RGBLED {
   public:
    RGBLED(PinName _red, PinName _green, PinName _blue);
    void writeRGB(int R, int G, int B);
    void setRGB(int R, int G, int B);
    void fadeRGB();
    void writeHSV(int H, int S, int V);
    void setHSV(int H, int S, int V);
    void fadeHSV();
    int rgb[3];
    int hsv[3];

   private:
    int targetRgb[3];
    int targetHsv[3];
};