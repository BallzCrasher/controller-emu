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

static struct KeyValuePair key_table[] = {
	KEY_PRESS(KEY_L, BTN_B),
	KEY_PRESS(KEY_J, BTN_X),
	KEY_PRESS(KEY_I, BTN_Y),
	KEY_PRESS(KEY_K, BTN_A)
};

#endif
