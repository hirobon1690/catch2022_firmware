#include "motor.h"

motor::motor(PinName pin, mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num,mcpwm_io_signals_t io_signal, PinName _dir0, PinName _dir1)
    : dir0(_dir0, OUTPUT), dir1(_dir1, OUTPUT), _mcpwm(pin, mcpwm_num, timer_num,io_signal) {
}

void motor::write(float duty) {
    if (duty<0){
        dir0.write(0);
        dir1.write(1);
    }else{
        dir0.write(1);
        dir1.write(0);
    }
    duty = abs(duty);
    duty > 100 ? duty = 100 : duty;
    _mcpwm.write(duty);
}