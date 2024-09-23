#ifndef CONFIG_H
#define CONFIG_H

#include <linux/input-event-codes.h>
#include "device.h"

#define GRAB_DEVICE 0
/* (measured in milliseconds) this will fix the problem
 * of mixed writing events as well as save cpu cycles when
 * the device is busy
 */
#define LATENCY 50

static int keypress_table[255] = {
/*  input event          output event     */
	[KEY_K]         =    BTN_B,
	[KEY_U]         =    BTN_X,
	[KEY_I]         =    BTN_Y,
	[KEY_J]         =    BTN_A,
	[KEY_O]         =    BTN_TR,
	[KEY_ENTER]     =    BTN_START,
	[KEY_BACKSLASH] =    BTN_SELECT
};

/* all of the available absolute event */
enum {
	NOEVENT,
	DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, DPAD_NEUTRAL,
	RJOY_UP, RJOY_DOWN, RJOY_LEFT, RJOY_RIGHT, RJOY_NEUTRAL,
	LJOY_UP, LJOY_DOWN, LJOY_LEFT, LJOY_RIGHT, LJOY_NEUTRAL
};

static int abspress_table[255] = {
/*  input event          output event     */
	[KEY_W] =            DPAD_UP,
	[KEY_S] =            DPAD_DOWN,
	[KEY_A] =            DPAD_LEFT,
	[KEY_D] =            DPAD_RIGHT,
};


static struct keypress_event abs_events[16] = {
	[DPAD_UP]      =  {.code = ABS_HAT0Y, .type = EV_ABS, .value = -1 },
	[DPAD_DOWN]    =  {.code = ABS_HAT0Y, .type = EV_ABS, .value =  1 },
	[DPAD_LEFT]    =  {.code = ABS_HAT0X, .type = EV_ABS, .value = -1 },
	[DPAD_RIGHT]   =  {.code = ABS_HAT0X, .type = EV_ABS, .value =  1 },
	[DPAD_NEUTRAL] =  {.code = ABS_HAT0X, .type = EV_ABS, .value =  0 },
};


#endif
