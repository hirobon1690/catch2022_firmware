#include "i2c.h"
#include "PinNames.h"

class gpioex{
    public:
        void mode(PinName _pin, PinMode mode);
        void set();
        void write(PinName _pin, bool val);
        bool read(PinName _pin);

    private:
        const uint8_t ADDR = 0x20;
        uint16_t input = 0x0000;
        uint16_t output = 0x0000;
        uint16_t config = 0x0000;    //リトルエンディアン
        uint16_t pullState = 0xFFFF;
};

extern gpioex ex;