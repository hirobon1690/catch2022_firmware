#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "i2c.h"
#include "twai.h"
#include "gpioex.h"

void enableCore0WDT();
void disableCore0WDT();
void delay_ms(int ms);
void init();