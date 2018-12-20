RetroSticks
===========

Connect two C64 joysticks to a PC via USB.

Introduction:
-------------

An Arduino Leonardo can act as a HID device when connected to a PC. Using MHeironimus'
excellent ArduinoJoystick library (https://github.com/MHeironimus/ArduinoJoystickLibrary),
you can build a simple interface to old C64 joysticks with no external parts (besides
two DSUB-9-male connectors for the joysticks).


Project status:
---------------

* works on Windows 10 and Linux (see notes below)

Components:
-----------

* Arduino Leonardo (others won't work, because they don't feature on-chip USB)
* 2 DSUB-9-male connectors

Hardware setup:
---------------

The controllers usually contain micro switches (one for each direction and a fire
button). When moving the stick into one direction, the corresponding switch is closed
and connects the associated direction pin to GND. Any pin with digital input capability
on the Arduino can be used to connect the switches. By configuring the pins as
inputs with an internal pull-up resistor, the switch state can easily be read from the
digital pin (0: switch is closed, 1: switch is open).

The actual mapping of controller switches to Arduino pins is defined in RetroSticks.h.
It was just dictated by the layout on my protoboard.

The DSUB-9 connector is used as follows:

    1: UP
    2: DOWN
    3: LEFT
    4: RIGHT
    5: Paddle Y (not used)
    6: FIRE
    7: +5V
    8: GND
    9: Paddle X (not used)

For a simple joystick like the classical Competition Pro, connecting 1-4, 6 and 8 would be
enough. To support sticks with additional gimmicks like auto-fire electronics, pin 7 should
be connected to the Arduino's +5V.

Required software libraries:
----------------------------

* https://github.com/MHeironimus/ArduinoJoystickLibrary

Usage on Windows
----------------

On Windows (tested on Windows 10), the device is detected as a game controller named
"Arduino Leonardo". This device contains two actual game pads which you can assign
separately e.g. when using an emulator like Vice.

Usage on Linux
--------------

On Linux (tested on Lubuntu 18.04 and RetroPie 4.4), the device will only be detected
as a single joystick. To make it work, you need to set a special parameter for the
usbhid kernel module. On Lubunte, I created a file "/etc/modprobe.d/usbhid-quirks.conf"
with the following content:

    options usbhid quirks=0x2341:0x8036:0x040

On the Raspberry Pi running RetroPie, usbhid is built-in. Therefore, you have to pass
the parameters on the kernel command line by adding them to /boot/cmdline.txt. Just
add

    usbhid.quirks=0x2341:0x8036:0x040

to the end of the single line in the file, separated from the other options by a blank.

In both cases, you have to reload the usbhid module or reboot the device to make it work.

Thanks:
-------

A big "thank you" goes to Matthew Heironimus for his ArduinoJoystick library that does most
of the work in this project.



