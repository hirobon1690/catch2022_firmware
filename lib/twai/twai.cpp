#include "twai.h"
#include <stdio.h>

_twai twai;

_twai::_twai() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX, CAN_RX, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
}

void _twai::init() {
    // Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }
    // Start TWAI driver
    if (twai_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
        return;
    }
}

void _twai::write(unsigned char id,unsigned char* data, size_t size) {
    twai_message_t msg;
    msg.identifier = id;
    msg.extd = 1;
    msg.data_length_code = size;
    for (int i = 0; i < size; i++) {
        msg.data[i] = *(data + i);
    }
    twai_transmit(&msg, pdMS_TO_TICKS(1000));
}

void _twai::read(twai_message_t* msg){
    twai_receive(msg, pdMS_TO_TICKS(10000));
}
