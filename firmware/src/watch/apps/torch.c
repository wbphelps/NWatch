/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

uint strobeV[] = {0, 50, 100, 200, 500, 1000};  // strobe delay times in ms
static uint strobe = 0;
static uint strobeI = 0;

static bool btnExit(void);
static bool btnFlashRateUp(void);
static bool btnFlashRateDown(void);
static display_t draw(void);

void torch_open()
{
	menu_close();

	strobe = 0;
	strobeI = 0;
	display_setDrawFunc(draw);
	buttons_setFuncs(btnFlashRateUp, btnFlashRateDown, btnExit);
}

static bool btnExit()
{
	oled_setInvert(appConfig.invert);
	display_load();
	return true;
}

static bool btnFlashRateUp()
{
	if (strobeI < 5)
	  strobeI++;
	else
		strobeI = 0;  // wrap around
  strobe = strobeV[strobeI];
	return true;
}

static bool btnFlashRateDown()
{
  if (strobeI > 0)
	  strobeI--;
	else
	  strobeI = 5;  // wrap around
	strobe = strobeV[strobeI];
	return true;
}

static display_t draw()
{
	static bool invert;
	static millis_t lastStrobe;

	if(strobe)
	{
		millis_t now = millis();
		if(now - lastStrobe >= strobe)
		{
			lastStrobe = now;
			invert = !invert;
			oled_setInvert(invert);
			led_flash(invert ? LED_GREEN : LED_RED, 20, 255);
		}
		return DISPLAY_BUSY;
	}

	oled_setInvert(true);
	led_flash(LED_GREEN, 100, 255);
	led_flash(LED_RED, 100, 255);

	return DISPLAY_DONE;
}
