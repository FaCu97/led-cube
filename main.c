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
	RC_Init();
	Cube_Init();
	
	while(1)
	{	
		switch ( Global_Variable&0x03)
		{
			case 0 : 
			{	
				Snow_Pattern(buffer); 
				break;
			};
			case 1 : 
			{
				Diagonal_Pattern(buffer); 
				break;
			};
			case 2 : 
			{
				Scale_Pattern(buffer); 
				break;
			};
			case 3 : 
			{
				Explode_Pattern(buffer); 
				break;
			};
		};
		Load_Buffer(buffer);
        _delay_ms(30);
	};
};




