#ifndef _SNAKE_H
#define _SNAKE_H

#include <avr/io.h>
#include "led_cube.h"
#include "remote_control.h"

#define SNAKE_LENGTH 3

enum snake_direction {DIR_X0, DIR_X1, DIR_Y0, DIR_Y1, DIR_Z0, DIR_Z1};
// Y0 - from player
// Y1 - towards player
// Z0 - up
// Z1 - down
// X0 - right
// X1 - left

typedef struct
{
	uint8_t x,y,z;
}snake_coordinates;

void Show_Snake(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], snake_coordinates snake[SNAKE_LENGTH]);
void Move_Snake(snake_coordinates snake[SNAKE_LENGTH], enum snake_direction dir);
enum snake_direction Change_Direction(enum snake_direction dir, uint8_t button);

#endif