#include "d4s7_74hc595.h"

#define NumSpi SPI2

unsigned char digit[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; //numbers

unsigned char column[4] = {0x01, 0x02, 0x04, 0x08}; //positions //,0x10,0x20,0x40,0x80  //8-digits position??

unsigned char digit_pointed[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10}; //numbers with dots

unsigned char letters[6] = {0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; //A, b, C, d, E, F

inline void Set595IntNumber(SPI_HandleTypeDef *hspi, uint16_t value) //CPU cycles: 24-32 (f303) (Ofast (best here)); 20-30 (Ofast) (f103); all without transfer
    {
    uint8_t buff[2];
    uint8_t count = 0;
    uint8_t zerr = 0;
    uint8_t digit_buf[4];

    digit_buf[0] = value % 10;
    digit_buf[1] = value % 100 / 10;
    digit_buf[2] = value % 1000 / 100;
    digit_buf[3] = value / 1000;

    if (value > 999)
	{
	zerr = 3;
	}
    else if (value > 99)
	{
	zerr = 2;
	}
    else if (value > 9)
	{
	zerr = 1;
	}
    else
	{
	zerr = 0;
	}

    for (count = 0; count <= zerr; count++)
	{
	buff[1] = column[count];
	buff[0] = digit[digit_buf[count]];

	//HAL_SPI_Transmit(hspi, (uint8_t*) &buff, 2, 10);
	HAL_SPI_Transmit_DMA(hspi, (uint8_t*) &buff, 2);

#ifdef CS_Soft

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

#endif
	HAL_Delay(2);
	}
    }
//#pragma GCC push_options
//#pragma GCC optimize ("Os")
void __attribute__((optimize("Os"))) Set595FloatNumber(SPI_HandleTypeDef *hspi, float value) //CPU cycles: 52-89 (f303); 162-407 (Os) (f103); all without transfer
    {
    uint8_t buff[2];
    // volatile //uncomment for -O1 -O2 -O3 -Ofast, but -Os make best speed ¯\_(ツ)_/¯
    uint8_t count = 0;
    uint8_t dot = 0;
    uint8_t digit_buf[4];

    if (value > 999)
	{
	dot = 0;
	}
    else if (value >= 100)
	{

	dot = 1;
	value *= 10;
	}
    else if (value >= 10)
	{
	dot = 2;
	value *= 100;
	}
    else
	{
	dot = 3;
	value *= 1000;
	}
    //#pragma GCC pop_options

    digit_buf[3] = (int) value / 1000;
    digit_buf[2] = (int) value % 1000 / 100;
    digit_buf[1] = (int) value % 100 / 10;
    digit_buf[0] = (int) value % 10;

    for (count = 0; count <= 4; count++)
	{
	if (count == dot && dot != 0)
	    {
	    buff[1] = column[count];
	    buff[0] = digit_pointed[digit_buf[count]];
	    }
	else
	    {
	    buff[1] = column[count];
	    buff[0] = digit[digit_buf[count]];
	    }

	//HAL_SPI_Transmit(hspi, (uint8_t*) &buff, 2, 10);
	HAL_SPI_Transmit_DMA(hspi, (uint8_t*) &buff, 2);

#ifdef CS_Soft

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

#endif
	HAL_Delay(1);
	}
    }
