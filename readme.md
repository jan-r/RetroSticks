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

* Port 1 works, and the Leonardo is identified as a single 2-axis-1-button gamepad

TODO:
-----

* Add second port and register a second USB device to make the device appear as two
  individual game pads.

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

Thanks:
-------

A big "thank you" goes to Matthew Heironimus for his ArduinoJoystick library that does most
of the work in this project.



