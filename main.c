#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_cube.h"
#include "patterns.h"
#include "snake.h"
#include "remote_control.h"

uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
snake_coordinates snake[MAX_SNAKE_LENGTH];
snake_coordinates snake_head, snake_tail;
snake_coordinates target;
enum snake_direction dir = DIR_X1;
uint8_t snake_lengthten_flag = 0;

int main(void)
{
	uint8_t i = 0;
	uint8_t temp;
	uint8_t snake_length = MIN_SNAKE_LENGTH;
	
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

	target.x = 0;
	target.y = 0;
	target.z = 0;

	snake_length = 1;
	
	snake[0].x = 2;
	snake[0].y = 1;
	snake[0].z = 2;
/*
	snake[1].x = 2;
	snake[1].y = 2;
	snake[1].z = 2;
*/
	snake_tail.x = 2;
	snake_tail.y = 0;
	snake_tail.z = 2;
	
	snake_head = snake[snake_length-1];

	dir = DIR_Y0;
	
	while (1)
	{	
	
	// Show_Snake
				
		Clear_Buffer(buffer);
		for (i = 0; i < snake_length; i++)
			buffer[snake[i].z][snake[i].x][snake[i].y] = BRIGHTNESS_MAX;	
		snake_head = snake[snake_length-1];
		
	// Create new target
		
		if (snake_lengthten_flag)
		{
			uint8_t r = random();
			target.x = 2;
			target.y = 2;
			target.z = 2;
		};
		
	// Smooth transition
		
		for (i = 1; i < BRIGHTNESS_MAX + 1; i++) // i = 0
		{
			buffer[target.z][target.z][target.z] = (BRIGHTNESS_MAX/2)*(i & 0x1);
			if (!snake_lengthten_flag)
				buffer[snake_tail.z][snake_tail.x][snake_tail.y] = BRIGHTNESS_MAX - i;
			buffer[snake_head.z][snake_head.x][snake_head.y] = i;
			
			Load_Buffer(buffer);
			_delay_ms(250/BRIGHTNESS_MAX);
			
		};
		snake_lengthten_flag = 0;
	
	// Move head of the snake
	
		temp = RC_Get_Command();
		if (temp)
			dir = Change_Direction(dir,temp);	
	
		switch (dir)
		{
			case DIR_X0:
				snake_head.x = (snake_head.x + 1) % CUBE_SIZE;
				break;
			case DIR_X1:
				snake_head.x = (snake_head.x - 1 + CUBE_SIZE) % CUBE_SIZE; // fix it !!!!
				break;
			case DIR_Y0:
				snake_head.y = (snake_head.y + 1) % CUBE_SIZE;
				break;
			case DIR_Y1:
				snake_head.y = (snake_head.y - 1 + CUBE_SIZE) % CUBE_SIZE;
				break;
			case DIR_Z0:
				snake_head.z = (snake_head.z + 1) % CUBE_SIZE;
				break;
			case DIR_Z1:
				snake_head.z = (snake_head.z - 1 + CUBE_SIZE) % CUBE_SIZE;
				break;
		};

	// Move snake body
	
		if (((snake_head.x == target.x) && (snake_head.y == target.y)) && (snake_head.z == target.z)) // increase length 
		{
			snake[snake_length] = snake_head;
			snake_length++;
			snake_lengthten_flag = 1;
		}
		else // shift snake
		{
			snake_tail = snake[0];
			for (i = 0; i < snake_length-1; i++)
				snake[i] = snake[i+1];
			snake[snake_length-1] = snake_head;
		}
		
			
	// If no target
	
		
				
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




