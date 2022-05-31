#include "i2c.h"
#include "gpioex.h"

void gpioex::mode(int pin, bool mode, bool pull){
    //modeが0で出力，0で入力，pullが0でプルダウン，1でプルアップ
    config=config|(mode<<pin);
    pullState=pullState|(pull<<pin);
}

void gpioex::set(){
    uint8_t cfg[3]={0x07,config>>8,config};
    i2c.write(ADDR,cfg,3);
    uint8_t pst[3]={0x49,pullState>>8,pullState};
    i2c.write(ADDR,pst,3);
}

void gpioex::write(int pin, bool val){
    output=output|(val<<pin);
    uint8_t command[3]={0x03,output>>8,output};
    i2c.write(ADDR,command,3);
}

bool gpioex::read(int pin){
    uint8_t result[2];
    i2c.write(ADDR, 0x01);
    i2c.read(ADDR, result, 2);
    input=result[0]<<8|result[1];
    return (input>>pin)&0x0001;
}