#include <driver/gpio.h>
#include <driver/twai.h>

class _twai {
   public:
    _twai();
    void init();
    void write(unsigned char id, unsigned char* data, size_t size);
    void read(twai_message_t* msg);

   private:
    const gpio_num_t CAN_TX = GPIO_NUM_23;
    const gpio_num_t CAN_RX = GPIO_NUM_25;
    twai_general_config_t g_config;
    twai_timing_config_t t_config;
    twai_filter_config_t f_config;
};

extern _twai twai;