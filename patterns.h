#ifndef _PATTERNS_H
#define _PATTERNS_H

#include "led_cube.h"	
#include <stdlib.h>

void Pattern (uint8_t n, uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Raindrops_Pattern  (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Diagonal_Pattern   (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Snow_Pattern       (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Swirl_Pattern      (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Scale_Pattern      (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Explode_Pattern    (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);


#endif
