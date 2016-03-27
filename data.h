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

#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>


void data_init(void);

void set_data_inside(int8_t temp, uint8_t hum);

void set_data_outside(int8_t temp, uint8_t hum);

uint8_t get_hum_inside(void);

uint8_t get_hum_outside(void);

uint8_t get_temp_inside();

uint8_t get_temp_outside();

uint8_t get_hum_diff(void);

void set_hum_diff(uint8_t diff);


#endif
