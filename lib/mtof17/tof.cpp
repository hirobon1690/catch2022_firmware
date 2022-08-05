#include "tof.h"

tof::tof(gpio &_select){
    select=_select;
    select.write(1);
}

int tof::read(){
    select.write(0);
    i2c.write(ADDR,0xD3);
    uint16_t result;
    i2c.read(ADDR,(uint8_t*)&result,2);
    return result;
    select.write(1);
}