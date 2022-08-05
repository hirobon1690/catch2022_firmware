#include "ads1015.h"

adc::adc(pins _pin):pin((int)_pin){
    conf[1]|=(pin<<4);
}

void adc::init(){

}

int adc::read(){
    i2c.write(ADDR,conf,3);
    uint16_t result;
    i2c.write(ADDR,0b00000000);
    i2c.read(ADDR,(uint8_t*)&result,2);
    return result;
}