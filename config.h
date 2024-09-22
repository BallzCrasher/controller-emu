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
	KEY_PRESS(KEY_KP4, BTN_B),
	KEY_PRESS(KEY_KP5, BTN_X),
	KEY_PRESS(KEY_KP6, BTN_Y),
	KEY_PRESS(KEY_KP3, BTN_A), 

	ABS_PRESS(KEY_UP,         ABS_HAT0Y, -1, 0),
	ABS_PRESS(KEY_DOWN,       ABS_HAT0Y,  1, 0),
	ABS_PRESS(KEY_LEFT,       ABS_HAT0X, -1, 0),
	ABS_PRESS(KEY_RIGHT,      ABS_HAT0X,  1, 0),
};

#endif
