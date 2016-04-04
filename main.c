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
#include "rtc3231.h"
#include "menu.h"
#include "lcd1602.h"

#define F_CPU 16000000UL
#define MAX_RTIME 28800

uint8_t now_day = 0;
volatile uint8_t is_run = 0;
volatile uint8_t time = 0;
volatile unsigned run_time = 0;


ISR (TIMER1_COMPA_vect)
{
	time++;
	if (is_run && (run_time != MAX_RTIME))
		run_time++;
}

int main(void)
{
	struct dht22 dht_out;
	struct dht22 dht_in;

	DDRB |= (1 << 3);
	PORTB &= ~(1 << 3);

	dht_init(&dht_out, 1);
	dht_init(&dht_in, 2);
	lcd1602_init();
	menu_init();
	data_init();
	rtc3231_init();

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

		if (time > 10) {
			uint8_t hd;
			float to, ho;
			float ti, hi;
			struct rtc_date dt;

			dht_read_data(&dht_out, &to, &ho);
			set_data_outside((int8_t)to, (uint8_t)ho);

			dht_read_data(&dht_in, &ti, &hi);
			set_data_inside((int8_t)ti, (uint8_t)hi);

			hd = get_hum_diff();
			if (ho < hi && (hi-ho) > hd && run_time < MAX_RTIME) {
				// on airing
				PORTB |= (1 << 3);
				is_run = 1;
			} else {
				// off airing
				PORTB &= ~(1 << 3);
				is_run = 0;
			}

			rtc3231_read_date(&dt);
			if (dt.day != now_day) {
				now_day = dt.day;
				run_time = 0;
			}

			menu_main_refresh();
			time = 0;
		}
	}
}
