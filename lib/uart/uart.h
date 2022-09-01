#pragma once
#include <driver/uart.h>

class _uart {
   public:
    void init();
    void enableIsr(void (*uartIsr)(void *));
    void write(char* val);
    void read(char* val);
    static intr_handle_t handle_console;
};

extern _uart uart;
