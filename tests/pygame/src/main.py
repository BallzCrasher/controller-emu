from pygame import joystick, event, display, constants

display.init()
event.pump()
if not joystick.get_init():
    joystick.init()


if joystick.get_count() == 0:
    print("not recognized")
    exit(1)

js = joystick.Joystick(0)
js.init()

print(js.get_name())
print(js.get_numbuttons())

while True:
    for ev in event.get():
        print(ev.dict)

