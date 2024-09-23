#ifndef CONFIG_H
#define CONFIG_H

#include <linux/input-event-codes.h>
struct keypress_event {
	int code;
	int type;
	int value;
};

struct KeyValuePair { 
	struct keypress_event inp;
	struct keypress_event out;
};

#define GRAB_DEVICE 0
// (measured in milliseconds) this will fix the problem
// of mixed writing events as well as save cpu cycles when
// the device is busy
#define LATENCY 50

#define KEY_PRESS(code_inp, code_out) \
{\
	.inp = { .code = (code_inp), .type = EV_KEY, .value = 0}, \
	.out = { .code = (code_out), .type = EV_KEY, .value = 0}  \
},\
{\
	.inp = { .code = (code_inp), .type = EV_KEY, .value = 1}, \
	.out = { .code = (code_out), .type = EV_KEY, .value = 1}  \
}

#define ABS_PRESS(code_inp, code_out, on_val, off_val) \
{\
	.inp = { .code = (code_inp), .type = EV_KEY, .value = 0}, \
	.out = { .code = (code_out), .type = EV_ABS, .value = (off_val)}  \
},\
{\
	.inp = { .code = (code_inp), .type = EV_KEY, .value = 1}, \
	.out = { .code = (code_out), .type = EV_ABS, .value = (on_val)}  \
}

static struct KeyValuePair key_table[] = {
	KEY_PRESS(KEY_K, BTN_B),
	KEY_PRESS(KEY_U, BTN_X),
	KEY_PRESS(KEY_I, BTN_Y),
	KEY_PRESS(KEY_J, BTN_A), 
	KEY_PRESS(KEY_O, BTN_TR), 
	KEY_PRESS(KEY_ENTER, BTN_START),
	KEY_PRESS(KEY_BACKSLASH, BTN_SELECT),

	ABS_PRESS(KEY_W, ABS_HAT0Y, -1, 0),
	ABS_PRESS(KEY_S, ABS_HAT0Y,  1, 0),
	ABS_PRESS(KEY_A, ABS_HAT0X, -1, 0),
	ABS_PRESS(KEY_D, ABS_HAT0X,  1, 0),
};

#endif
