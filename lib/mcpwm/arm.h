#pragma once
#include "ads1015.h"
#include "gpio.h"
#include "motor.h"
#include "commonfunc.h"

class arm {
   public:
    arm(motor& _motor, gpio& _s0, gpio& _s1, adc& _pot, int _max);
    void home(float speed,int min=0,int max=0);
    float getDeg();
    float calDeg(int);

   private:
    motor& m0;
    adc& pot;
    gpio& s0;
    gpio& s1;
    int max;
    int rawData[2];
};