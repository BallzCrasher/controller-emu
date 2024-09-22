#ifndef DEVICE_H
#define DEVICE_H

int init_device(int dev);
int destroy_device(int dev);

#define JOYSTICK_ABS_MIN -32768
#define JOYSTICK_ABS_MAX  32767

#define RT_ABS_MIN 0
#define RT_ABS_MAX 255

#define HAT_ABS_MIN -1
#define HAT_ABS_MAX  1

#endif
