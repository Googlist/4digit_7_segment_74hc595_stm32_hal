#ifndef d4s7_74hc595_H_
#define d4s7_74hc595_H_

#include "stm32f1xx_hal.h"
#include "main.h"

#define CS_Soft


void Set595IntNumber (SPI_HandleTypeDef *hspi, uint16_t value);

#endif // 4D7S74HC595_H
