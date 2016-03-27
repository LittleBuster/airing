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

#ifndef __MENU_H__
#define __MENU_H__

#define B_MENU_MAIN 5
#define B_MENU_PLUS 0
#define B_MENU_MINUS 7
#define B_MENU_OK 6

#define BTN_DELAY 400


void menu_init(void);

void menu_main_refresh(void);

void menu_loop(void);


#endif
