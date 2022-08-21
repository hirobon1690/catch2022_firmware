#include "mcpwm.h"
/**
 * @brief This function initializes each gpio signal for MCPWM
 *        @note
 *        This function initializes one gpio at a time.
 *
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param io_signal set MCPWM signals, each MCPWM unit has 6 output(MCPWMXA, MCPWMXB) and 9 input(SYNC_X, FAULT_X, CAP_X)
 *                  'X' is timer_num(0-2)
 * @param gpio_num set this to configure gpio for MCPWM, if you want to use gpio16, gpio_num = 16
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
mcpwm::mcpwm(PinName pin,mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num,mcpwm_io_signals_t io_signal):MCPWM_NUM(mcpwm_num),IO_SIGNAL(io_signal),TIMER_NUM(timer_num){
    mcpwm_gpio_init(MCPWM_NUM, IO_SIGNAL, (int)pin);
    mcpwm_config_t pwmconfig;
    pwmconfig.frequency = 100000;
    pwmconfig.cmpr_a = 0;  // duty cycle of PWMxA = 0
    pwmconfig.counter_mode = MCPWM_UP_COUNTER;
    pwmconfig.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_NUM, TIMER_NUM, &pwmconfig);
    mcpwm_set_frequency(MCPWM_NUM, TIMER_NUM, 10000);
}

void mcpwm::write(float duty){
    mcpwm_set_duty(MCPWM_NUM, TIMER_NUM, MCPWM_OPR_A, duty);
}