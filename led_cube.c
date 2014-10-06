#include <avr/io.h>
#include <avr/interrupt.h>
#include "led_cube.h"
#include <avr/io.h>


volatile uint8_t layer_counter = 0;
volatile uint8_t brightness_counter = 0;
volatile uint8_t data_ready[CUBE_SIZE][BRIGHTNESS_MAX][2];



void Brightness_Mask(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], uint16_t *mask, uint8_t brightness)
{
	uint8_t i,j;
	for (i = 0; i < CUBE_SIZE; i++)
	{
		j = 0;
		while (mask[i])
		{
			if (mask[i] & 0x01)
				buffer[i][j/CUBE_SIZE][j%CUBE_SIZE] = brightness;
			mask[i]>>=1;
			j++;
		};
	};
};




void Load_Buffer_0(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	uint8_t i1,i3,i0;
	
	DEBUG2_ON;
	for (i1 = 0; i1 < CUBE_SIZE; i1++) // height
	{
		for (i3 = 0; i3 < CUBE_SIZE; i3++) // depth
		{
			for (i0 = 0; i0 < BRIGHTNESS_MAX; i0++)
			{
				if (buffer[i1][0][i3] > i0)
					data_ready[i1][i0][0] |= (0x01<<i3);
				else
					data_ready[i1][i0][0] &=~(0x01<<i3);

				if (buffer[i1][1][i3] > i0)
					data_ready[i1][i0][0] |= (0x10<<i3);
				else
					data_ready[i1][i0][0] &=~(0x10<<i3);

				if (buffer[i1][2][i3] > i0)
					data_ready[i1][i0][1] |= (0x80>>i3);
				else
					data_ready[i1][i0][1] &=~(0x80>>i3);

				if (buffer[i1][3][i3] > i0)
					data_ready[i1][i0][1] |= (0x08>>i3);
				else
					data_ready[i1][i0][1] &=~(0x08>>i3);
			};
		};
	};
	DEBUG2_OFF;
};


/*
void Load_Buffer_1(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	uint8_t i1,i2,i3,i0;
	
	for (i1 = 0; i1 < CUBE_SIZE; i1++) // height
	{
		for (i2 = 0; i2 < BRIGHTNESS_MAX; i2++)
		{
			data_ready[i1][i2][0] = 0x00;
			data_ready[i1][i2][1] = 0x00;
		};
		for (i2 = 0; i2 < CUBE_SIZE; i2++) // width
		{
			for (i3 = 0; i3 < CUBE_SIZE; i3++)
			{
				uint8_t brightness = buffer[i1][i2][i3];
				if (brightness > BRIGHTNESS_MAX)
					brightness = BRIGHTNESS_MAX;
				for (i0 = 0; i0 < brightness; i0++)
				{
					switch (i2)
					{
						case 0 : data_ready[i1][i0][0] |= (0x01<<i3); break;
						case 1 : data_ready[i1][i0][0] |= (0x10<<i3); break;
						case 2 : data_ready[i1][i0][1] |= (0x80>>i3); break;
						case 3 : data_ready[i1][i0][1] |= (0x08>>i3); break;
					};
				};
			};
		};
	};
};
*/



void Invert_Axis(uint16_t mask[CUBE_SIZE], uint8_t axis)
{
	uint8_t i;
	uint16_t tmp;

	if (axis & 0x04)
		for (i = 0; i < CUBE_SIZE/2; i++)
		{
			tmp = mask[i];
			mask[i] = mask[CUBE_SIZE - i - 1];
			mask[CUBE_SIZE - i - 1] = tmp; 
		};

	if (axis & 0x02)
		for (i = 0; i < CUBE_SIZE; i++)
		{
			tmp = 0;
			tmp |= (mask[i] & 0xF000) >> 12;
			tmp |= (mask[i] & 0x0F00) >> 4;
			tmp |= (mask[i] & 0x00F0) << 4;
			tmp |= (mask[i] & 0x000F) << 12;
			mask[i] = tmp;
		};

	if (axis & 0x01)
		for (i = 0; i < CUBE_SIZE; i++)
		{
			tmp = 0;
			tmp |= (mask[i] & 0x8888) >> 3;
			tmp |= (mask[i] & 0x4444) >> 1;
			tmp |= (mask[i] & 0x2222) << 1;
			tmp |= (mask[i] & 0x1111) << 3;
			mask[i] = tmp;
		};
};



void Create_Diagonal_Mask(uint16_t mask[CUBE_SIZE], uint8_t cnt)
{
	uint8_t ix,iy,iz;
	for (iz = 0; iz < CUBE_SIZE; iz++)
		mask[iz] = 0;

	for (ix = 0; ix < CUBE_SIZE; ix++)
		for (iy = 0; iy < CUBE_SIZE; iy++)
			for (iz = 0; iz < CUBE_SIZE; iz++)
				if ((ix+iy+iz) == cnt)
					mask[3-iz] |= (1 << (iy*4 + ix));
};


void Clear_Buffer(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	uint8_t ix,iy,iz;

	for (ix = 0; ix < CUBE_SIZE; ix++)
		for (iy = 0; iy < CUBE_SIZE; iy++)
			for (iz = 0; iz < CUBE_SIZE; iz++)
				buffer[iz][ix][iy] = 0;
};



ISR(TIMER2_COMP_vect)
{ 
	DEBUG1_ON;
	//cli();
	TIMSK &= ~(1<<OCIE2);
	PORTD = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;

	if (brightness_counter < BRIGHTNESS_MAX-1)
	{
		//if (brightness_counter > 0)
		OCR2<<=1;
		brightness_counter++;
	}
	else
	{
		if (layer_counter < CUBE_SIZE-1)
		{
			layer_counter++;
		}
		else
		{
			layer_counter = 0;
		};
		brightness_counter = 0;
		OCR2 = OCR2_DEFAULT;
	};
	PORTB = data_ready[layer_counter][brightness_counter][0];
	PORTC = data_ready[layer_counter][brightness_counter][1];
	PORTD = 0x10 << layer_counter;
	TCNT2 = 0;
	//sei();
	TIMSK |= (1<<OCIE2);
	DEBUG1_OFF;
};




void Cube_Init(void)
{
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	DDRB  = 0xFF;
	DDRC  = 0xFF;
	DDRD  = 0xF0;
	PORTA &= ~(DEBUG_PIN1 | DEBUG_PIN2);
	DDRA = DEBUG_PIN1 | DEBUG_PIN2;

	TCCR2 = (1<<WGM21) | (1<<CS22) | (1<<CS21) | (0<<CS20); // Prescaler, Clear timer on Compare match;
	OCR2 = OCR2_DEFAULT;
	TIMSK |= (1<<OCIE2); // Timer2 output compare Interrupt;	
	sei();
};
