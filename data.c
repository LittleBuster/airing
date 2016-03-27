/* Auto-airing system for garage
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "data.h"
#include <avr/eeprom.h>


static struct {
	uint8_t hum_diff;
	uint8_t hum_inside;
	uint8_t hum_outside;
	uint8_t temp_inside;
	uint8_t temp_outside;
} data = {
		.hum_diff = 10,
		.hum_inside = 0,
		.hum_outside = 0,
		.temp_inside = 0,
		.temp_outside = 0,
		.hum_inside = 0,
		.hum_outside = 0
};


void data_init(void)
{
	data.hum_diff = eeprom_read_byte((uint8_t *)0x01);
}

void set_data_inside(int8_t temp, uint8_t hum)
{
	data.temp_inside = temp;
	data.hum_inside = hum;
}

void set_data_outside(int8_t temp, uint8_t hum)
{
	data.temp_outside = temp;
	data.hum_outside = hum;
}

uint8_t get_hum_inside(void)
{
	return data.hum_inside;
}

uint8_t get_hum_outside(void)
{
	return data.hum_outside;
}

uint8_t get_temp_inside()
{
	return data.temp_inside;
}

uint8_t get_temp_outside()
{
	return data.temp_outside;
}

uint8_t get_hum_diff(void)
{
	return data.hum_diff;
}

void set_hum_diff(uint8_t diff)
{
	data.hum_diff = diff;
	eeprom_write_byte((uint8_t *)0x01, diff);
}
