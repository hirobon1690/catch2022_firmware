#include "servo.h"

servo::servo(PinName pin, int min, int max) {
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.duty_resolution = LEDC_TIMER_12_BIT;
    ledc_timer.freq_hz = 50;  // Set output frequency at 5 kHz
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num = (int)pin;
    ledc_channel.duty = 0;  // Set duty to 0%
    ledc_channel.hpoint = 0;

    ledc_channel_config(&ledc_channel);
}

void servo::write(float angle) {
    // uint32_t duty=(int)(0.000001*angle*angle*angle-0.0008*angle*angle+1.2481*angle+104);
    // uint32_t duty=(int)(-0.00004*angle*angle+1.1228*angle+110); //2次近似
    uint32_t duty=(int)(-0.0000004*angle*angle*angle+0.00003*angle*angle+1.1731*angle+102); //3次近似

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}


void servo::duty(uint32_t duty) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}