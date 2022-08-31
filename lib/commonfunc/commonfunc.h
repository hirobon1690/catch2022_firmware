#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void enableCore0WDT();
void disableCore0WDT();
void disableCore1WDT();
void delay_ms(int ms);