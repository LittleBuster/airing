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

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "dht22.h"
#include "data.h"
#include "menu.h"
#include "lcd1602.h"

#define F_CPU 16000000UL

volatile uint8_t time = 0;


ISR (TIMER1_COMPA_vect)
{
	time++;
}

int main(void)
{
	struct dht22 dht_out;
	struct dht22 dht_in;

	dht_init(&dht_out, 1); // out
	dht_init(&dht_in, 2); // in
	lcd1602_init();
	menu_init();
	data_init();

	lcd1602_clear();
	lcd1602_send_string("  Airing System");
	lcd1602_goto_xy(0, 1);
	lcd1602_send_string("      2016");

	_delay_ms(2000);
	lcd1602_clear();

	// Timer run
	TCCR1A = 0x00;
	TCCR1B = (1 << CS12)|(0 << CS11)|(1 << CS10)|(1 << WGM12); //1024
	TCNT1 = 0x00;
	OCR1A = 15625; // 16kk/1024
	TIMSK |= (1 << OCIE1A);
	sei();

	for (;;) {
		menu_loop();

		if (time > 5) {
			float t, h;

			dht_read_data(&dht_out, &t, &h);
			set_data_outside((int8_t)t, (uint8_t)h);

			dht_read_data(&dht_in, &t, &h);
			set_data_inside((int8_t)t, (uint8_t)h);

			menu_main_refresh();
			time = 0;
		}
	}
}
