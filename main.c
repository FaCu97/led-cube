#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_cube.h"
#include "patterns.h"

uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
uint16_t mask[CUBE_SIZE];

#define TIME 300

int main(void)
{
	uint16_t i;

	Cube_Init();
	
	while(1)
	{
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Snow_Pattern(buffer);
			Load_Buffer_0(buffer);
        	_delay_ms(30);
		};
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Diagonal_Pattern(buffer);
			Load_Buffer_0(buffer);
        	_delay_ms(30);
		};
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Scale_Pattern(buffer);
			Load_Buffer_0(buffer);
        	_delay_ms(30);
		};
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Explode_Pattern(buffer);
			Load_Buffer_0(buffer);
        	_delay_ms(30);
		};


	};
};




