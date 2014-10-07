#include "patterns.h"
#include "led_cube.h"
#include <stdio.h>

uint8_t counter = 0;
uint8_t sub_counter = 0;
uint16_t mask[4] = {0,0,0,0};
uint8_t density = 1;

void Pattern(uint8_t n, uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	static uint8_t current_pattern = 0;

	if (n != current_pattern)
	{
		Clear_Buffer(buffer);
		current_pattern = n;
		counter = 0;
		sub_counter = 0;
		density = 1;
	};

	switch (n)
	{
		case 0 :
			Snow_Pattern(buffer); 
			break;
		case 1 :
			Diagonal_Pattern(buffer); 
			break;
		case 2 : 
			Scale_Pattern(buffer); 
			break;
		case 3 : 
			Explode_Pattern(buffer); 
			break;
		case 4 : 
			Raindrops_Pattern(buffer); 
			break;
		case 5 : 
			Swirl_Pattern(buffer); 
			break;
	};
};


void Raindrops_Pattern(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	static uint8_t pos_cnt = 0;
	static uint8_t pos[CUBE_SIZE*CUBE_SIZE] = { 0, 5, 12, 3, 11, 9, 8, 15, 1, 10, 4, 7, 13, 2, 6, 14 };
	static uint8_t source_layer[CUBE_SIZE][CUBE_SIZE];
	uint8_t ix,iy,iz;

	if (counter == 3)
	{
		ix = pos[pos_cnt] / CUBE_SIZE;
		iy = pos[pos_cnt] % CUBE_SIZE;
		source_layer[ix][iy] = 2;

		counter = 0;

		pos_cnt++;
		if (pos_cnt == CUBE_SIZE*CUBE_SIZE)
			pos_cnt = 0;
	};

	counter++;
	
	for (iz = 0; iz < CUBE_SIZE; iz++)
		for (ix = 0; ix < CUBE_SIZE; ix++)
			for (iy = 0; iy < CUBE_SIZE; iy++)
				if (((iz < (CUBE_SIZE-1)) && (buffer[iz+1][ix][iy] > 0)) || ((iz == (CUBE_SIZE-1)) && (source_layer[ix][iy] > 0)))
				{
					if (buffer[iz][ix][iy] < BRIGHTNESS_MAX)
						buffer[iz][ix][iy]++;
				}
				else
					if (buffer[iz][ix][iy] > 0)
						buffer[iz][ix][iy]--;

	for (ix = 0; ix < CUBE_SIZE; ix++)
		for (iy = 0; iy < CUBE_SIZE; iy++)
			if (source_layer[ix][iy] > 0)
				source_layer[ix][iy]--;
};




void Diagonal_Pattern(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	#define DIAGONAL_SIZE  ((CUBE_SIZE-1)*3 + 1)	

	uint8_t i;	
	static uint8_t angle = 0;	

	for (i = 0; i < CUBE_SIZE; i++)
		mask[i] = 0xFFFF;
	Brightness_Mask(buffer,mask,0); // clear all;

	for (i = BRIGHTNESS_MAX; i > 0 ; i--)
	{
		Create_Diagonal_Mask(mask,counter + (i-BRIGHTNESS_MAX+1) - BRIGHTNESS_MAX);
		Invert_Axis(mask,angle);
		Brightness_Mask(buffer, mask, i);
		Create_Diagonal_Mask(mask,counter - (i-BRIGHTNESS_MAX+1) - BRIGHTNESS_MAX);
		Invert_Axis(mask,angle);
		Brightness_Mask(buffer, mask, i);
	};

	counter++;

	if (counter == BRIGHTNESS_MAX*2 + DIAGONAL_SIZE + 1)
	{
		counter = 0;
		angle ^= 0x03;
	};

};

void Snow_Pattern(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	#define SNOW_VALUE_MAX  (BRIGHTNESS_MAX)	
	static uint16_t fading_away[4] = {0,0,0,0};
	static uint8_t direction = 0;
	uint8_t random_number;
	uint8_t ix,iy,iz;
	uint8_t i;

	for (ix = 0; ix < CUBE_SIZE; ix++)
		for (iy = 0; iy < CUBE_SIZE; iy++)
			for (iz = 0; iz < CUBE_SIZE; iz++)
			{
				if (buffer[iz][ix][iy])
				{
					if (!(fading_away[iz] & (1 << (4*iy + ix))))
						buffer[iz][ix][iy]++;
					else
						buffer[iz][ix][iy]--;
				};
				
				if (buffer[iz][ix][iy] == SNOW_VALUE_MAX)
					fading_away[iz] |= (1 << (4*iy + ix));

				if (buffer[iz][ix][iy] == 0)
					fading_away[iz] &=~(1 << (4*iy + ix));
			};


	if (counter & 0x01)
	for (i = 0; i < density; i++)
	{			
		do
		{
			random_number = rand();
			iz = (random_number & 0x30) >> 4;			
			ix = (random_number & 0x0C) >> 2;
			iy = (random_number & 0x03);
		} 
		while (buffer[iz][ix][iy]);
		buffer[iz][ix][iy] = 1;
	};


	counter++;

	if (counter > 3)
	{	
		counter = 0;
		if (!direction)
			density++;		
		else
			density--;
		
		if (density > 10)
			direction = 1;
		if (density == 0)
			direction = 0;
	};
};

void Swirl_Pattern(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
};


void Scale_Pattern(uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	static uint8_t direction = 0;
	static uint8_t inv_axis = 0;
	static uint8_t angle = 0;

	#define SCALE_VALUE_MAX (BRIGHTNESS_MAX - 1)

	switch (counter)
	{
		case 0 : 
		case 7 :
			{mask[0] = 0x0001; mask[1] = 0x0000; mask[2] = 0x0000; mask[3] = 0x0000; break; };
		case 1 :
		case 6 : 
			{mask[0] = 0x0032; mask[1] = 0x0013; mask[2] = 0x0000; mask[3] = 0x0000; break; };
		case 2 : 
		case 5 :
			{mask[0] = 0x0744; mask[1] = 0x0104; mask[2] = 0x0117; mask[3] = 0x0000; break; };
		case 3 :
		case 4 : 
			{mask[0] = 0xF888; mask[1] = 0xF888; mask[2] = 0xF888; mask[3] = 0xFFFF; break; };
	};
	Invert_Axis(mask, inv_axis);

	if (!direction)	
		Brightness_Mask(buffer,mask,sub_counter);
	else
		Brightness_Mask(buffer,mask, SCALE_VALUE_MAX - sub_counter);

	sub_counter++;

	if (sub_counter > SCALE_VALUE_MAX) 
	{
		sub_counter = 0;
		counter++;
			
		if (counter % CUBE_SIZE == 0)
		{
			direction ^= 1;
			inv_axis = (angle ^ 0x07);
		};

		if (counter == CUBE_SIZE*2)
		{
			mask[0] = 0xFFFF;		
			mask[1] = 0xFFFF;		
			mask[2] = 0xFFFF;		
			mask[3] = 0xFFFF;
			Brightness_Mask(buffer,mask,0);		
			counter = 0;
			angle++;
			if (angle == 4)
				angle = 0;
			inv_axis = angle;
		};
	};
};





void Explode_Pattern (uint8_t buffer[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE])
{
	static uint8_t direction = 0;

	#define EXPLODE_VALUE_MAX (BRIGHTNESS_MAX - 1)

	switch (counter%4)
	{
		case 0 : 
			{ mask[0] = 0x0000; mask[1] = 0x0660; mask[2] = 0x0660; mask[3] = 0x0000; break; };
		case 1 :
			{ mask[0] = 0x0660; mask[1] = 0x6996; mask[2] = 0x6996; mask[3] = 0x0660; break; };
		case 2 :
			{ mask[0] = 0x6996; mask[1] = 0x9009; mask[2] = 0x9009; mask[3] = 0x6996; break; };
		case 3 :
			{ mask[0] = 0x9009; mask[1] = 0x0000; mask[2] = 0x0000; mask[3] = 0x9009; break; };
	};

	if (!direction)	
		Brightness_Mask(buffer,mask,sub_counter);
	else
		Brightness_Mask(buffer,mask, EXPLODE_VALUE_MAX - sub_counter);

	sub_counter++;

	if (sub_counter > EXPLODE_VALUE_MAX) 
	{
		sub_counter = 0;
		counter++;			
		if (counter % 4 == 0)
			direction ^= 1;
		if (counter == 8)	
			counter = 0;
	};
};
