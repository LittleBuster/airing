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

#include "sound.h"
#include <avr/io.h>
#include <util/delay.h>


void short_signal(void)
{
	DDRC |= (1 << SOUND_PIN);
	PORTC |= (1 << SOUND_PIN);
	_delay_ms(50);
	PORTC &= ~(1 << SOUND_PIN);
	DDRC &= ~(1 << SOUND_PIN);
}
