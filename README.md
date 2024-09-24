# controller-emu
A lightweight Linux utility that can turn input events into game controllers.

## Motivation
Many games that I play require an extra controller if I want to play with my friends.
However, even if I bought a controller, I feel that the keyboard player has an advantage.
So, I decided to create a program that can transform raw keyboard events to a raw controller 
events using uinput and data extracted from my friend's controller.

## Features
- Turn event of a keyboard into events to events of a controller
- This can be done to as many keyboards as you like.
- The Ability to grab a keyboard so that it can only be used by this utility.
- No need to split the keys between. As each keyboard has separate events.
- Easy to compile using any decent C compiler and require no extra dependencies.
- Can be easily modified using the config.h file

## Building
You can change the C compiler in the Makefile if you like (default is gcc).
```bash
https://github.com/BallzCrasher/controller-emu.git
cd controller-emu
make
```
This will create the binary "controller-emu".

## Running
First check that uinput is probed in your kernel. If it's not you must modprobe it using:
```bash
sudo modprobe uinput
```
Then you can run controller-emu using:
```bash
sudo ./controller-emu /path/to/dev
```

The second argument must be a character device that corresponds to the keyboard
you want to be using. (typically found in /dev/input/).
you can determine which one through a variety of ways easiest of which is using evtest.
A way you can do it without installing anything is by running: 

```bash
 cat /proc/bus/input/devices | grep -E '(N:|H:)'
```
and get the handler for the device you want. for example if the output of the above command is:
```
...
N: Name="AT Translated Set 2 keyboard"
H: Handlers=sysrq kbd leds event2
...
```
Then the character device is /dev/input/event2

## Configuration
All configurations are in config.h, That include key bindings and the option to grab devices.
All key and button code are in input-event-codes.h in your system.
If you wish to determine which code your keyboard generates, I recommend using evtest.

## Notes
This command requires root privileges to create a device using uinput, And for
reading raw data from the character device. If, you want to run it without root.
You must add a udev rule allowing yourself to access uinput. And join the group that
belongs to the character devices (typically, it is the "input" group).
