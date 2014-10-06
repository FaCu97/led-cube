#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_cube.h"
#include "patterns.h"
#include "remote_control.h"

extern unsigned char Global_Variable;
uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
uint16_t mask[CUBE_SIZE];

#define TIME 300

int main(void)
{
	//uint16_t i;
	RC_Init();
	Cube_Init();
	
	while(1)
	{	
		_delay_ms(100);
		Clear_Buffer(buffer);
		buffer[0][0][Global_Variable&0x03] = BRIGHTNESS_MAX;

		Load_Buffer(buffer);

		
		Clear_Buffer(buffer); 
		/*
		for (i = 0; i < TIME; i++)
		{
			Snow_Pattern(buffer);
			Load_Buffer(buffer);
        	_delay_ms(30);
		};
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Diagonal_Pattern(buffer);
			Load_Buffer(buffer);
        	_delay_ms(30);
		};
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Scale_Pattern(buffer);
			Load_Buffer(buffer);
        	_delay_ms(30);
		};
		Clear_Buffer(buffer);
		for (i = 0; i < TIME; i++)
		{
			Explode_Pattern(buffer);
			Load_Buffer(buffer);
        	_delay_ms(30);
		};
		*/

		


	};
};




