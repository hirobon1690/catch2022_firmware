#include "i2c.h"
#include "PinNames.h"

class gpioex{
    public:
        void mode(PinName _pin, PinMode mode);
        void set();
        void write(PinName _pin, bool val);
        bool read(PinName _pin);

    private:
        const uint8_t ADDR = 0x18;
        uint8_t input = 0x00;
        uint8_t output = 0x00;
        uint8_t config = 0x00;    //リトルエンディアン
        // uint8_t pullState = 0xFF;
};

extern gpioex ex;