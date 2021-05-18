#include "d4s7_74hc595.h"

#define NumSpi SPI2

unsigned char digit[10] =
    {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
    //numbers
	};
unsigned char column[4] =
    {
    0x01, 0x02, 0x04, 0x08
    //positions
    //,0x10,0x20,0x40,0x80  //8-digits position??
	};

unsigned char digit_pointed[10] =
    {
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10
    };

unsigned char letters[6] =
    {
    0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
    }; //A, b, C, d, E, F

void Set595IntNumber(SPI_HandleTypeDef *hspi, uint16_t value);
void Set595IntNumber(SPI_HandleTypeDef *hspi, uint16_t value)
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
    else
	{
	if (value > 99)
	    {
	    zerr = 2;
	    }
	else
	    {
	    if (value > 9)
		{
		zerr = 1;
		}
	    else
		{
		zerr = 0;
		}
	    }
	}

    for (count = 0; count <= zerr; count++)
	{
	buff[1] = column[count];			//posizione
	buff[0] = digit[digit_buf[count]];	     	//numero

	HAL_SPI_Transmit(hspi, (uint8_t*) &buff, 2, 10);
	//HAL_SPI_Transmit_DMA(hspi, (uint8_t*) &buff, 2);

#ifdef CS_Soft

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

#endif
	HAL_Delay(1);
	}
    }

