#include "i2c.h"

class adc{
    public:
        adc(pins _pin);
        void init();
        int read();

    private:
        int pin;
        uint8_t conf[3]={0b00000001,0b10000010,0b10000011};
        const int ADDR=0x48;
};

enum pins{
    A0=0b100,
    A1=0b101,
    A2=0b110,
    A3=0b111
};