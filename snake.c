#include "snake.h"


void Show_Snake(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], snake_coordinates snake[], uint8_t snake_length)
{
	
}

void Move_Snake( snake_coordinates snake[], enum snake_direction dir , uint8_t snake_length)
{

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

void Snake_Delay(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], uint8_t target[3])
{

};