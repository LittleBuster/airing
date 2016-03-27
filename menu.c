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
#include <stdio.h>
#include <string.h>
#include "lcd1602.h"
#include "sound.h"
#include "menu.h"
#include "data.h"


static struct {
	uint8_t level;

	uint8_t menu;
	uint8_t is_diff;
	uint8_t is_time;
	uint8_t is_main;
	uint8_t is_edit;

	uint8_t diff_cfg;
} menu = {
	.menu = 0,
	.level = 0,
	.is_diff = 0,
	.is_time = 0,
	.is_main = 0,
	.diff_cfg = 0,
	.is_edit = 0
};

void menu_init(void)
{
	DDRD &= ~(1 << B_MENU_MAIN);
	DDRB &= ~(1 << B_MENU_PLUS);
	DDRD &= ~(1 << B_MENU_MINUS);
	DDRD &= ~(1 << B_MENU_OK);
	PORTD |= (1 << B_MENU_MAIN);
	PORTB |= (1 << B_MENU_PLUS);
	PORTD |= (1 << B_MENU_MINUS);
	PORTD |= (1 << B_MENU_OK);

}

void menu_main_refresh(void)
{
	if (menu.level == 0) {
		menu.is_main = 0;
		lcd1602_clear();
	}
}

void menu_loop(void)
{
	if ((PIND & (1 << B_MENU_MAIN)) == 0) {
		if (menu.level == 0) {
			menu.level++;
			menu.menu = 1;
		} else {
			menu.menu = 0;
			menu.level = 0;
			menu.is_diff = 0;
			menu.is_time = 0;
			menu.is_main = 0;
		}
		_delay_ms(BTN_DELAY);
		lcd1602_clear();
	}

	if ((PINB & (1 << B_MENU_PLUS)) == 0 && menu.menu && !menu.is_edit) {
		if (menu.level < 2)
			menu.level++;
		else
			menu.level = 1;
		lcd1602_clear();
		_delay_ms(BTN_DELAY);

		menu.is_diff = 0;
		menu.is_time = 0;
	}

	if ((PIND & (1 << B_MENU_MINUS)) == 0 && menu.menu && !menu.is_edit) {
		if (menu.level > 1)
			menu.level--;
		else
			menu.level = 2;
		lcd1602_clear();
		_delay_ms(BTN_DELAY);

		menu.is_diff = 0;
		menu.is_time = 0;
	}

	switch (menu.level) {
		case 0: {
			if (!menu.is_main) {
				char sdata[5];

				lcd1602_send_string("OtH InH OtT InT");
				lcd1602_goto_xy(0, 1);
				memset(sdata, 0x00, 4);
				snprintf(sdata, 4, "%d%%", get_hum_outside());
				lcd1602_send_string(sdata);
				lcd1602_goto_xy(4, 1);
				memset(sdata, 0x00, 4);
				snprintf(sdata, 4, "%d%%", get_hum_inside());
				lcd1602_send_string(sdata);
				lcd1602_goto_xy(8, 1);
				memset(sdata, 0x00, 4);
				snprintf(sdata, 4, "%dC", get_temp_inside());
				lcd1602_send_string(sdata);
				lcd1602_goto_xy(12, 1);
				memset(sdata, 0x00, 4);
				snprintf(sdata, 4, "%dC", get_temp_outside());
				lcd1602_send_string(sdata);
				menu.is_main++;
			}
			break;
		}

		case 1: {
			if (!menu.is_diff) {
				char out[16];
				memset(out, 0x00, 14);

				lcd1602_send_string(" Hum Difference");
				lcd1602_goto_xy(0, 1);

				uint8_t hd = get_hum_diff();

				if (!menu.is_edit)
					snprintf(out, 14, "       %d%%", hd);
				else
					snprintf(out, 14, "      [%d%%]", hd);
				lcd1602_send_string(out);
				menu.is_diff++;
			}

			if ((PINB & (1 << B_MENU_PLUS)) == 0 && menu.is_edit) {
				_delay_ms(BTN_DELAY);
				lcd1602_clear();
				menu.is_diff--;
				uint8_t hd = get_hum_diff();
				hd++;
				set_hum_diff(hd);
			}

			if ((PIND & (1 << B_MENU_MINUS)) == 0 && menu.is_edit) {
				_delay_ms(BTN_DELAY);
				lcd1602_clear();
				menu.is_diff--;
				uint8_t hd = get_hum_diff();
				hd--;
				set_hum_diff(hd);
			}


			if ((PIND & (1 << B_MENU_OK)) == 0 && menu.menu) {
				lcd1602_clear();
				_delay_ms(500);
				if (!menu.is_edit) {
					menu.is_edit++;
					menu.is_diff--;
				}
				else {
					menu.is_edit--;
					menu.is_diff--;
					//write_to_eeprom
				}
			}
			break;
		}

		case 2: {
			if (!menu.is_time) {
				lcd1602_send_string("       Time");
				lcd1602_goto_xy(0, 1);
				lcd1602_send_string("     Settings");
				menu.is_time++;
			}
			break;
		}
	}
}
