#include <driver/uart.h>

class _uart {
   public:
    void init();
    void write(char* val);
    void read(char* val);
};

extern _uart uart;
