/*
 * RetroSticks
 *
 * A C64-joystick-to-USB-converter based on an Arduino Leonardo.
 * When connected to a PC, it identifies as a HID device with two
 * 2-axis-1-button joysticks.
 *
 * This sketch uses Matthew Heironimus' Arduino Joystick library
 * (https://github.com/MHeironimus/ArduinoJoystickLibrary) and
 * is based on the GamepadExample sketch that comes with this lib.
 *
 * This file is licensed under the BSD 3-clause license.
 *
 * Copyright 2018 Jan Reucker
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Joystick.h>

// Set this define to output events to the serial port instead of acting like
// a real joystick. In that case, no USB HID device will be created. Open the
// Arduino IDE's serial monitor to see the joystick's events.
//#define SERIAL_DEBUGGING

// Hardware setup - port 1
#define PORT1_UP    A0
#define PORT1_DOWN  A1
#define PORT1_LEFT  A2
#define PORT1_RIGHT A3
#define PORT1_BTN   A4

// Hardware setup - port 2
#define PORT2_UP    D6
#define PORT2_DOWN  D5
#define PORT2_LEFT  D4
#define PORT2_RIGHT D3
#define PORT2_BTN   D7

// Debouncing filter for the input. Defines the number of times a pin
// must keep a steady value before being recognized by the software.
#define FILTER_MAX  2

// value order in indexed arrays
#define IDX_UP    0
#define IDX_DOWN  1
#define IDX_LEFT  2
#define IDX_RIGHT 3
#define IDX_BTN   4

// events
#define EV_RELEASE    0
#define EV_PRESS      1

#ifndef SERIAL_DEBUGGING
// The USB joystick device
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  1, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering
#endif

// input map (maps port pins to the logical values; same order as the IDX_... constants)
int inputMap[5] = {PORT1_UP, PORT1_DOWN, PORT1_LEFT, PORT1_RIGHT, PORT1_BTN};

// input filter for debouncing the raw pin input
int8_t buttonFilter[5] = {FILTER_MAX,FILTER_MAX,FILTER_MAX,FILTER_MAX,FILTER_MAX};

// Last state of the switches
int8_t buttonState[5] = {0,0,0,0,0};

// Arduino standard setup function
void setup()
{
  // Initialize Button Pins
  for (int8_t index = 0; index < 5; index++)
  {
    pinMode(inputMap[index], INPUT_PULLUP);
  }

#ifndef SERIAL_DEBUGGING
  // Initialize Joystick Library
  Joystick.begin(false);
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
#else
  Serial.begin(115200);
#endif  
}

// Arduino standard loop function
void loop()
{
  bool changed = false;
  
  // Read pin values
  for (int8_t index = 0; index < 5; index++)
  {
    // read pin
    int8_t currentPinState = digitalRead(inputMap[index]);

    // debounce pin
    if ((currentPinState == 0) && (buttonFilter[index] > 0))
    {
      // pin LOW (button pressed)
      buttonFilter[index]--;
    }
    else if ((currentPinState == 1) && (buttonFilter[index] < FILTER_MAX))
    {
      // pin HIGH (button not pressed)
      buttonFilter[index]++;
    }

    // evaluate state change
    if ((buttonState[index] == 1) && (buttonFilter[index] == FILTER_MAX))
    {
      // button released
      buttonEvent(index, EV_RELEASE);
      changed = true;
      buttonState[index] = 0;
    }
    else if ((buttonState[index] == 0) && (buttonFilter[index] == 0))
    {
      // button pressed
      buttonEvent(index, EV_PRESS);
      changed = true;
      buttonState[index] = 1;
    }
    
  }
  if (changed)
  {
#ifndef SERIAL_DEBUGGING
    Joystick.sendState();
#endif
  }
  delay(5);
}

// Convert press/release events of the individual buttons into
// axis movements of the HID device.
void buttonEvent(int8_t index, int8_t event)
{
  switch(index)
  {
    case IDX_UP:
      if (event == EV_PRESS)
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setYAxis(-1);
        #else
        Serial.println("UP p");
        #endif
      }
      else
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setYAxis(0);
        #else
        Serial.println("UP r");
        #endif
      }
      break;

    case IDX_DOWN:
      if (event == EV_PRESS)
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setYAxis(1);
        #else
        Serial.println("DN p");
        #endif
      }
      else
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setYAxis(0);
        #else
        Serial.println("DN r");
        #endif
      }
      break;

      case IDX_LEFT:
      if (event == EV_PRESS)
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setXAxis(-1);
        #else
        Serial.println("LFT p");
        #endif
      }
      else
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setXAxis(0);
        #else
        Serial.println("LFT r");
        #endif
      }
      break;

    case IDX_RIGHT:
      if (event == EV_PRESS)
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setXAxis(1);
        #else
        Serial.println("RGT p");
        #endif
      }
      else
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setXAxis(0);
        #else
        Serial.println("RGT r");
        #endif
      }
      break;

    case IDX_BTN:
      if (event == EV_PRESS)
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setButton(0, 1);
        #else
        Serial.println("BTN p");
        #endif
      }
      else
      {
        #ifndef SERIAL_DEBUGGING
        Joystick.setButton(0, 0);
        #else
        Serial.println("BTN r");
        #endif
      }
      break;

    default:
      break;
  }
}

