#include "gpio.h"
#include "mcpwm.h"

class motor {
   public:
    motor(PinName pin, mcpwm_unit_t mcpwm_num, mcpwm_io_signals_t io_signal, PinName dir0,PinName dir1);
    void write(int duty);

   private:
    gpio dir0;
    gpio dir1;
    mcpwm mcpwm;
};