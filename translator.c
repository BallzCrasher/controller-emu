#include <unistd.h>
#include <linux/input.h>

#include "device.h"
#include "config.h"
#include "translator.h"

struct keypress_event* lookup(struct input_event ev) {
	static int dpad_xdir = 0, dpad_ydir = 0;
	static int rjoy_xdir = 0, rjoy_ydir = 0;
	static int ljoy_xdir = 0, ljoy_ydir = 0;
	static struct keypress_event res;

	/* remove duplicate events */
	if (ev.value == 2) 
		return NULL;

	if (ev.code == ESCAPE_KEY) {
		res.type = -1;
		return &res;
	}

	/* keypress are return after mapping to correct button */
	if (keypress_table[ev.code] != 0) {
		res.type  = ev.type;
		res.code  = keypress_table[ev.code];
		res.value = ev.value;
		return &res;
	}

	/* absolute presses must be checked for conflicting data*/
	if (abspress_table[ev.code] != NOEVENT) {
		int code = abspress_table[ev.code];
		
		/* only releases of the same diraction can cancel movement.
		 * i.e the release of left key must not cancel the press 
		 * of the right key.
		 */
		if (code == DPAD_LEFT || code == DPAD_RIGHT) {
			if (ev.value == 1) {
				dpad_xdir = abs_events[code].value;
				return &abs_events[code];
			}

			if (ev.value == 0 && dpad_xdir == abs_events[code].value) {
				dpad_xdir = 0;
				return &abs_events[DPAD_XNEUTRAL];
			}
		}

		if (code == DPAD_UP || code == DPAD_DOWN) {
			if (ev.value == 1) {
				dpad_ydir = abs_events[code].value;
				return &abs_events[code];
			}

			if (ev.value == 0 && dpad_ydir == abs_events[code].value) {
				dpad_ydir = 0;
				return &abs_events[DPAD_YNEUTRAL];
			}
		}

		//TODO support other absolute events
	}

	return NULL;
}
