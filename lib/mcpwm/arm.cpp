#include "arm.h"

arm::arm(motor& _motor, gpio& _s0, gpio& _s1, adc& _pot, int _max):m0(_motor), pot(_pot), s0(_s0), s1(_s1), max(_max){
}

void arm::home(float speed) {
    m0.write(-speed);
    delay_ms(100);
    m0.write(speed);
    while (1) {
        if (!s0.read()) {
            break;
        }
        delay_ms(1);
    }
    m0.write(0);
    rawData[0] = pot.readAvrg(100);
    printf("%d\n", rawData[0]);
    delay_ms(100);
    m0.write(-speed);
    while (1) {
        if (!s1.read()) {
            break;
        }
        delay_ms(1);
    }
    m0.write(0);
    rawData[1] = pot.readAvrg(100);
    printf("%d\n", rawData[1]);
    m0.write(speed);
    delay_ms(100);
    m0.write(0);
}

float arm::getDeg(){
    int raw=pot.readAvrg(10);
    return max*(float)raw/(rawData[0]-rawData[1])+(float)rawData[1]*max/(rawData[1]-rawData[0]);
}

float arm::calDeg(int raw){
    return max*(float)raw/(rawData[0]-rawData[1])+(float)rawData[1]*max/(rawData[1]-rawData[0]);
}