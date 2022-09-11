#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "pca9557.h"
#include "i2c.h"
#include "twai.h"
#include "uart.h"

void enableCore0WDT();
void disableCore0WDT();
void enableCore1WDT();
void disableCore1WDT();
void delay_ms(int ms);
void init();