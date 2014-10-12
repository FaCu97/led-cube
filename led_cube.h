
#ifndef _LED_CUBE_H
#define _LED_CUBE_H

#include <avr/io.h>

#define BRIGHTNESS_MAX 	6
#define CUBE_SIZE   	4
#define	OCR2_DEFAULT   	1
#define HALF_PERIMETER  (CUBE_SIZE*2-2)

#define	DEBUG_PIN1	(1<<3) // green
#define	DEBUG_PIN2	(1<<4) // white
#define DEBUG1_ON	PORTA |= DEBUG_PIN1
#define DEBUG1_OFF	PORTA &= ~DEBUG_PIN1
#define DEBUG2_ON	PORTA |= DEBUG_PIN2
#define DEBUG2_OFF	PORTA &= ~DEBUG_PIN2

void Cube_Init(void);
void Brightness_Mask(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], uint16_t mask[CUBE_SIZE], uint8_t brightness);

void Load_Buffer(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);
void Invert_Axis(uint16_t mask[CUBE_SIZE], uint8_t axis);
void Create_Diagonal_Mask(uint16_t mask[CUBE_SIZE], uint8_t cnt);
void Clear_Buffer(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE]);

#endif
