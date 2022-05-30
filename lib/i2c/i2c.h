#pragma once
#include <driver/i2c.h>

class i2c{
    public:
        void init();
        void write(uint8_t addr, uint8_t* data,size_t size);
        void read(uint8_t addr,uint8_t* data, size_t size);

    private:
        int i2c_master_port=0;
};
