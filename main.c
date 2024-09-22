#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

#include "config.h"
#include "device.h"
#include "translator.h"

int emit(int fd, int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   return write(fd, &ie, sizeof(ie));
}

void handle_error(const char* str) {
	fprintf(stderr, "Error: %s (%s)\n", str, strerror(errno)); 
	_exit(EXIT_FAILURE);
}


void signal_handler(int signum) {
	_exit(0); // _exit closes file descriptors
}

int main() {
	// get the characted device descriptor from uinput
	int dev = open("/dev/uinput", O_NONBLOCK | O_WRONLY | O_SYNC);
	if (dev < 0)
		handle_error("Unable to create device");

	// get the file descriptor for the input device
	int inp = open("/dev/input/event3", O_RDONLY | O_NONBLOCK);
	if (inp < 0)
		handle_error("Unable to open input device");

	signal(SIGINT, signal_handler);

	// initialize the device with predefined metadata
	if (init_device(dev) == -1)
		handle_error("Unable to initialize uinput device");

	struct input_event event;
	int res;
	// read input event from the input device
	while ((res = read(inp, &event, sizeof(event))) != 0) {
		if (res == -1) {
			if (errno == EAGAIN) {
				continue;
			}
			handle_error("input device read fail");
		}

		// if event is in the lookup table emit the corrosponding event
		struct keypress_event* ev = lookup(event);
		if (ev != NULL) {
			// sync
			if (emit(dev, EV_SYN, SYN_REPORT, 0) == -1) {
				handle_error("could not send syn report");
			}

			if (emit(dev, ev->type, ev->code, ev->value) == -1) {
				handle_error("could not emit event");
			}
		}
	}

	// close the device
	if (destroy_device(dev))
		handle_error("unable to destroy device");
	close(inp);

	return EXIT_SUCCESS;
}
