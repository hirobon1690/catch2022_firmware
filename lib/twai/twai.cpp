#include "twai.h"
#include <stdio.h>

_twai twai;

_twai::_twai() {
    g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX, CAN_RX, TWAI_MODE_NORMAL);
    t_config = TWAI_TIMING_CONFIG_500KBITS();
    f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
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
    twai_transmit(&msg, portMAX_DELAY);
}

void _twai::write(twai_message_t* msg) {
    twai_transmit(msg, portMAX_DELAY);
}

void _twai::read(twai_message_t* msg){
    twai_receive(msg, portMAX_DELAY);
}
