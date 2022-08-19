#include <driver/mcpwm.h>
#include "PinNames.h"

class mcpwm {
   public:
    mcpwm(PinName pin,mcpwm_unit_t mcpwm_num, mcpwm_io_signals_t io_signal);
    void write(int duty);

   private:
    mcpwm_unit_t MCPWM_NUM;
    mcpwm_io_signals_t IO_SIGNAL;
};