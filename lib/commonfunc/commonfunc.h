#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void enableCore0WDT();
void disableCore0WDT();
void delay_ms(int ms);