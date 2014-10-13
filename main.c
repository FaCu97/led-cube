#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_cube.h"
#include "patterns.h"
#include "snake.h"
#include "remote_control.h"

uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
snake_coordinates snake[4];
enum snake_direction dir = DIR_X1;

int main(void)
{
	uint8_t i = 0;
	uint8_t temp;
	
	RC_Init();
	Cube_Init();

/*	
	while (1)
	{
		temp = RC_Get_Command();
		switch (temp)
		{
			case BTN_RIGHT: 
				if (i < N_OF_PATTERNS-1)
					i++; 
				else
					i = 0;
				break;
			case BTN_LEFT: 
				if (i > 0)
					i--;
				else
					i = N_OF_PATTERNS-1;
				break;
		}		
		Pattern(i,buffer);
		Load_Buffer(buffer);
		_delay_ms(30);
	}
*/


// SNAKE GAME

	for (i = 0; i < SNAKE_LENGTH; i++)
	{
		snake[i].x = 2;
		snake[i].y = 2;
		snake[i].z = i;
	}

	dir = DIR_X1;
	while (1)
	{
		Show_Snake(buffer, snake);
		_delay_ms(500);
		temp = RC_Get_Command();
		if (temp)
			dir = Change_Direction(dir,temp);	
		Move_Snake(snake, dir);
	};


/*	while(1)
	{	
		temp = RC_Get_Command();
		if (temp)
			dir = temp;

		switch (dir)
		{
			case BTN_UP     : x++; break;
			case BTN_DOWN   : x--; break;
			case BTN_RIGHT  : y++; break;
			case BTN_LEFT   : y--; break;
			case BTN_CENTER : z++; break;
			case BTN_MENU   : z--; break;
			case BTN_PLAY   : b = ((((b/2) - 1)^1) + 1) * 2;
		};
		Clear_Buffer(buffer);
		buffer[z%4][y%4][x%4] = b;
		Load_Buffer(buffer);
		_delay_ms(300);
		
		//Load_Buffer(buffer);
        //_delay_ms(100);		
		//Pattern(Global_Variable&0x03, buffer);
	};*/
};




