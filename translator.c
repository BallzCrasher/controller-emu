#include <unistd.h>
#include <linux/input.h>
#include "config.h"
#include "translator.h"

struct keypress_event* lookup(struct input_event ev) {

	int table_len = sizeof(key_table) / sizeof(struct KeyValuePair);

	for (int i = 0; i < table_len; i++) {
		if (   key_table[i].inp.code == ev.code
			&& key_table[i].inp.type == ev.type
			&& key_table[i].inp.value == ev.value)
		{
			return &key_table[i].out;
		}

	}
	return NULL;
}
