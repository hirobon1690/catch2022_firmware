#include "led.h"

RGBLED::RGBLED(PinName red, PinName green, PinName blue) {
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_1;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz = 1000;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel = LEDC_CHANNEL_1;
    ledc_channel.timer_sel = LEDC_TIMER_1;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num = (int)red;
    ledc_channel.duty = 0;  // Set duty to 0%
    ledc_channel.hpoint = 0;

    ledc_channel_config(&ledc_channel);

    ledc_channel.channel = LEDC_CHANNEL_2;
    ledc_channel.timer_sel = LEDC_TIMER_1;
    ledc_channel.gpio_num = (int)green;
    ledc_channel_config(&ledc_channel);

    ledc_channel.channel = LEDC_CHANNEL_3;
    ledc_channel.timer_sel = LEDC_TIMER_1;
    ledc_channel.gpio_num = (int)blue;
    ledc_channel_config(&ledc_channel);
}

void RGBLED::writeRGB(int R, int G, int B) {
    rgb[0] = R;
    rgb[1] = G;
    rgb[2] = B;
    R = 255 - R;
    G = 255 - G;
    B = 255 - B;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 1023 * R / 255);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 1023 * G / 255);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 1023 * B / 255);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);
}

void RGBLED::writeHSV(int H, int S, int V) {
    hsv[0] = H;
    hsv[1] = S;
    hsv[2] = V;
    int R = 0, G = 0, B = 0;
    float MAX = V;
    float MIN = MAX - (((float)S / 255) * MAX);
    if (H < 60) {
        R = MAX;
        G = ((float)H / 60) * (MAX - MIN) + MIN;
        B = MIN;
    } else if (H < 120) {
        R = (((float)120 - H) / 60) * (MAX - MIN) + MIN;
        G = MAX;
        B = MIN;
    } else if (H < 180) {
        R = MIN;
        G = MAX;
        B = ((float)(H - 120) / 60) * (MAX - MIN) + MIN;
    } else if (H < 240) {
        R = MIN;
        G = ((float)(240 - H) / 60) * (MAX - MIN) + MIN;
        B = MAX;
    } else if (H < 300) {
        R = ((float)(H - 240) / 60) * (MAX - MIN) + MIN;
        G = MIN;
        B = MAX;
    } else if (H < 360) {
        R = MAX;
        G = MIN;
        B = ((float)(360 - H) / 60) * (MAX - MIN) + MIN;
    }
    this->writeRGB(R, G, B);
}