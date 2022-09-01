#include "tof.h"

tof::tof(gpio& _select):select(_select){
    select.write(0);
}

int tof::read(){
    // select.write(1);
    i2c.write(ADDR,0xD3);
    uint8_t result[2];
    i2c.read(ADDR,result,2);
    // select.write(0);
    return result[0]<<8|result[1];
}