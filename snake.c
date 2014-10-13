#include "snake.h"


void Show_Snake(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], snake_coordinates snake[SNAKE_LENGTH])
{
	uint8_t i;	
	Clear_Buffer(buffer);
	for (i = 0; i < SNAKE_LENGTH; i++)
	{
		buffer[snake[i].z][snake[i].x][snake[i].y] = BRIGHTNESS_MAX;
	};
	Load_Buffer(buffer);	
}

void Move_Snake( snake_coordinates snake[SNAKE_LENGTH], enum snake_direction dir )
{
	snake_coordinates temp;
	uint8_t i;
	for (i = (SNAKE_LENGTH-1); i > 0; i--)
	{
		snake[i] = snake[i-1];
	};
			
	switch (dir)
	{
		case DIR_X0:
			snake[0].x = (snake[0].x + 1) % CUBE_SIZE;
			break;
		case DIR_X1:
			snake[0].x = (snake[0].x - 1 + CUBE_SIZE) % CUBE_SIZE; // fix it !!!!
			break;
		case DIR_Y0:
			snake[0].y = (snake[0].y + 1) % CUBE_SIZE;
			break;
		case DIR_Y1:
			snake[0].y = (snake[0].y - 1 + CUBE_SIZE) % CUBE_SIZE;
			break;
		case DIR_Z0:
			snake[0].z = (snake[0].z + 1) % CUBE_SIZE;
			break;
		case DIR_Z1:
			snake[0].z = (snake[0].z - 1 + CUBE_SIZE) % CUBE_SIZE;
			break;
	};			
};


enum snake_direction Change_Direction(enum snake_direction dir, uint8_t button)
{
	switch (dir)
	{
		case DIR_X0:
		case DIR_X1:
		{
			switch (button)
			{
				case BTN_CENTER :
					return DIR_Z0;
				case BTN_MENU :
					return DIR_Z1;
				case BTN_UP :
					return DIR_Y0;
				case BTN_DOWN :
					return DIR_Y1;
				default :
					return dir;
			};
			break;
		};	
		case DIR_Y0:
		case DIR_Y1:
		{
			switch (button)
			{
				case BTN_CENTER :
					return DIR_Z0;
				case BTN_MENU :
					return DIR_Z1;
				case BTN_RIGHT :
					return DIR_X0;
				case BTN_LEFT :
					return DIR_X1;
				default :
					return dir;
			};
			break;
		};
		case DIR_Z0:
		case DIR_Z1:
		{
			switch (button)
			{
				case BTN_UP :
					return DIR_Y0;
				case BTN_DOWN :
					return DIR_Y1;
				case BTN_RIGHT :
					return DIR_X0;
				case BTN_LEFT :
					return DIR_X1;
				default :
					return dir;
			};
			break;
		};
	};
};
