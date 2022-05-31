#include "i2c.h"

class gpioex{
    public:
        void mode(int pin, bool mode, bool pull);
        void set();
        void write(int pin, bool val);
        bool read(int pin);

    private:
        const uint8_t ADDR = 0x20;
        uint16_t input = 0x0000;
        uint16_t output = 0x0000;
        uint16_t config = 0x0000;    //リトルエンディアン
        uint16_t pullState = 0xFFFF;
};
