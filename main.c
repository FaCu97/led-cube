#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_cube.h"
#include "patterns.h"
#include "remote_control.h"

extern unsigned char Global_Variable;
uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

int main(void)
{
	RC_Init();
	Cube_Init();
	
	while(1)
	{	
		Load_Buffer(buffer);
        _delay_ms(100);
		Pattern(Global_Variable&0x03, buffer);
	};
};




