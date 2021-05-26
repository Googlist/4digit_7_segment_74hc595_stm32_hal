#ifndef d4s7_74hc595_H_
#define d4s7_74hc595_H_

#include "stm32f1xx_hal.h"
//#include "stm32f3xx_hal.h"
#include "main.h"

#define CS_Soft //f103 do not work CS hard, f303 work


void Set595IntNumber (SPI_HandleTypeDef *hspi, uint16_t value);
void Set595FloatNumber(SPI_HandleTypeDef *hspi, float value);

#endif
