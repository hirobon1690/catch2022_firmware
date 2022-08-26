#pragma once
#include "ads1015.h"
#include "commonfunc.h"

adc::adc(pins _pin):pin((int)_pin){
    this->conf[1]|=(pin<<4);
}

void adc::init(){

}

int adc::read(){
    i2c.write(ADDR,conf,3);
    uint8_t result[2];
    uint8_t data=0b00000000;
    i2c.write(ADDR,&data,1);
    i2c.read(ADDR,result,2);
    return (result[0]<<4)|(result[1]>>4);
}

int adc::readAvrg(int times){
    int result=0;
    for(int i=0;i<times;i++){
        result+=this->read();
    }
    return result/times;
}