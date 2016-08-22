# Pushbutton library for Arduino

Version: 2.0.0 <br>
Release date: 2016-08-22 <br>
[![Build Status](https://travis-ci.org/pololu/pushbutton-arduino.svg?branch=master)](https://travis-ci.org/pololu/pushbutton-arduino) <br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for the Arduino IDE that helps interface with pushbuttons by
either reading the state of the button or monitoring it for press/release
events.

## Supported platforms

This library is designed to work with the Arduino IDE versions 1.0.x and 1.5.x,
and will probably not work with older versions.

This library supports any [Arduino-compatible](https://www.pololu.com/arduino)
board.

## Getting started

### Hardware

This library supports many different ways of connecting a button to your board.
The simplest way is to select a pin that is not being used for anything else and
to connect a
[normally-open momentary pushbutton](http://www.pololu.com/product/1400) from
that pin to ground.

### Software

If you are using version 1.6.2 or later of the
[Arduino software (IDE)](https://www.arduino.cc/en/Main/Software), you can use
the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then
   "Manage Libraries...".
2. Search for "Pushbutton".
3. Click the Pushbutton entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/pushbutton-arduino/releases)
   and decompress it.
2. Rename the folder "pushbutton-arduino-xxxx" to "Pushbutton".
3. Drag the "Pushbutton" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Creating a Pushbutton object

To create a Pushbutton object with default settings, which enables the internal
pull-up on the pin and interprets a high pin value as the default (unpressed)
state of the button, put these lines near the top of your sketch:

~~~{.cpp}
#include <Pushbutton.h>
Pushbutton button(BUTTON_PIN);
~~~

Optional arguments can be passed to the constructor to specify other button
types and connection methods; see the documentation links below for details.

## Reading the state of the button

The `isPressed()` function can be used to directly read the state of the button.
This function simply calls `digitalRead` to get the current state of the button
and does not take care of any debouncing.

~~~{.cpp}
if (button.isPressed())
{
  // The button is currently pressed.
}
~~~

## Waiting for an event

The Pushbutton class has several functions that wait for an event on the button.
These functions take care of debouncing for you.

The `waitForPress()` function waits until the button is pressed.  The
`waitForRelease()` function waits until the button is released.

Note that if the button is already pressed when `waitForPress()` is called, it
will return quickly (in 10 ms).  If you want to perform an action when the user
presses the button but want to avoid performing the action multiple times while
the user holds the button down, you will need to call `waitForRelease()`:

~~~{.cpp}
void loop() {
  button.waitForPress();
  // Perform action here.
  button.waitForRelease();
}
~~~

The `waitForButton()` function waits until the button is pressed, and then waits
until the button is released.  This function could be used like this:

~~~{.cpp}
void loop() {
  button.waitForButton();
  // Perform action here.
}
~~~

These functions wait for an event to happen before returning, so your board
cannot do much else while it is waiting.  They are appropriate for simple
programs where a button press is the only event you need to respond to in the
main loop.

## Monitoring for an event

The Pushbutton class provides two non-blocking functions that allow you to
monitor the button for transitions.  These functions are powerful and can be
used in almost any situation.

The `getSingleDebouncedPress()` function will return true once for each time it
detects the button changing from the released state to the pressed state.  The
`getSingleDebouncedRelease()` function will return true once for each time it
detects the button changing from the pressed state to the released state.

These functions are non-blocking and are meant to be called repeatedly in a
loop:

~~~{.cpp}
void loop() {
  if (button.getSingleDebouncedPress())
  {
    // The button was pressed, so perform some action.
  }
  // Perform other tasks
}
~~~

## Documentation

For complete documentation of this library, including many features that were
not mentioned here, see
[the Pushbutton.h file documentation](https://pololu.github.io/pushbutton-arduino/_pushbutton_8h.html)
from https://pololu.github.io/pushbutton-arduino.

## Version history

* 2.0.0 (2016-08-22): Updated library to work with the Arduino Library Manager.
* 1.1.1 (2016-06-03):
    * Fixed some small errors in the documentation.
* 1.1.0 (2014-12-11):
    * Added the PushbuttonBase class, which allows custom pushbutton classes with their own unique ways of reading the state of the button.
    * Changed all booleans (which are actually uint8_t) to bools.
    * Fixed a problem in the logic for getSingleDebouncedPress().
    * Added the PushbuttonStateMachine class to reduce code duplication.
    * Moved the library to its own repository at https://github.com/pololu/pushbutton-arduino.
* 1.0.1 (2014-01-22): Fixed it to work properly with multiple instances.  Improved the debouncing logic and reduced the width of PrevTimeMillis variables. This release was from commit 8c4e8f5 in pololu/zumo-shield.
* 1.0.0 (2012-11-09): Original release.  At this time, the Pushbutton library lived in https://github.com/pololu/zumo-shield and this release was from commit a02cf00 in pololu/zumo-shield.
