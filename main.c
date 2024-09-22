#include <errno.h>
#include <linux/input-event-codes.h>
#include <stdlib.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <assert.h>

static int setup_abs(int dev, int code, int value, int min, int max, int flat, int fuzz) {
	struct uinput_abs_setup abs_setup;
	memset(&abs_setup, 0, sizeof(abs_setup));

	abs_setup.code               = code;
	abs_setup.absinfo.flat       = flat;
	abs_setup.absinfo.fuzz       = fuzz;
	abs_setup.absinfo.minimum    = min;
	abs_setup.absinfo.maximum    = max;
	abs_setup.absinfo.value      = value;
	return ioctl(dev, UI_ABS_SETUP, &abs_setup);
}

static int emit(int dev, int type, int code, int value) {
		struct input_event ev;
		memset(&ev, 0, sizeof(ev));
		ev.type = EV_KEY;
		ev.code = code; 
		ev.value = value;
		fprintf(stderr, "Writing event: code=%d, value=%d to device\n", ev.code, ev.value);
		return write(dev, &ev, sizeof(ev));
}

// dev must be an open fd for uinput
static int init_device(int dev) {

	// list of available keys for the device
	const int supported_keybits[] = {
		BTN_JOYSTICK,
		BTN_A,
		BTN_B,
		BTN_Y,
		BTN_X,
		BTN_TL,
		BTN_TR,
		BTN_SELECT,
		BTN_START,
		BTN_MODE,
		BTN_THUMBL,
		BTN_THUMBR
	};
	int supported_keybits_len = sizeof(supported_keybits) / sizeof(int);
	for (int i = 0; i < supported_keybits_len; i++) {
		if (ioctl(dev, UI_SET_KEYBIT, supported_keybits[i]) == -1)
			return -1;
	}

	// enable absolute movement of joysticks
	if (ioctl(dev, UI_SET_EVBIT, EV_ABS) == -1)
		return -1;
	if (setup_abs(dev, ABS_X, 0, -32768, 32767, 128, 16) == -1)
		return -1;
	if (setup_abs(dev, ABS_Y, -1, -32768, 32767, 128, 16) == -1)
		return -1;
	if (setup_abs(dev, ABS_RX, 0, -32768, 32767, 128, 16) == -1)
		return -1;
	if (setup_abs(dev, ABS_RY, -1, -32768, 32767, 128, 16) == -1)
		return -1;

	// enable absolute movement for LT and RT
	if (setup_abs(dev, ABS_Z, 0, 0, 255, 0, 0) == -1)
		return -1;
	if (setup_abs(dev, ABS_RZ, -1, 0, 255, 0, 0) == -1)
		return -1;

	// Dpad is also absolute because up = -down and right = -left
	if (setup_abs(dev, ABS_HAT0X, 0, -1, 1, 0, 0) == -1)
		return -1;
	if (setup_abs(dev, ABS_HAT0Y, 0, -1, 1, 0, 0) == -1)
		return -1;

	// setting up metadata for device
	struct uinput_setup uisetup;
	memset(&uisetup, 0, sizeof(uisetup));
	strcpy(uisetup.name, "8BitDo Pro 2 Wired Controller");
	uisetup.id.bustype = BUS_USB;
	uisetup.id.vendor = 0x2dc8;
	uisetup.id.product = 0x3106;
	uisetup.id.version = 0x114;

	// creating the device
	if (ioctl(dev, UI_DEV_SETUP, &uisetup) == -1)
		return -1;
	if (ioctl(dev, UI_DEV_CREATE) == -1)
		return -1;

	sleep(1); // Wait for device recognition

	return dev;
}

static int destroy_device(int dev) {
	if (ioctl(dev, UI_DEV_DESTROY) == -1)
		return -1;
	return 0;
}

static void handle_error(int dev) {
	close(dev); 
	fprintf(stderr, "Error: %s\n", strerror(errno)); 
	exit(EXIT_FAILURE);
}

int main() {
	int dev = open("/dev/uinput", O_NONBLOCK | O_WRONLY);
	if (dev < 0) {
		fprintf(stderr, "Error Opening file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	if (init_device(dev) == -1)
		handle_error(dev);

	const int btns[] = { BTN_A, BTN_B, BTN_X, BTN_Y };

	struct event_t {
		int type;
		int code;
		int value;
	} event;

	int res;
	while ((res = read(stdin, &event, sizeof(event))) != 0) {
		if (res == -1)
			handle_error(dev);

		emit(dev, event.type, event.code, event.value);
	}

	if (destroy_device(dev))
		handle_error(dev);

	return EXIT_SUCCESS;
}
