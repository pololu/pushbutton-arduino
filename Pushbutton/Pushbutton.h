// Copyright (C) Pololu Corporation.  See LICENSE.txt for details.

/*! \file Pushbutton.h
 *
 * This is the main header file for the Pushbutton library.
 *
 * For an overview of the library's features, see
 * https://github.com/pololu/pushbutton-arduino.  That is the main repository
 * for the library, though copies of the library may exist in other
 * repositories.
 *
 */

#ifndef Pushbutton_h
#define Pushbutton_h

#include <Arduino.h>

#define PULL_UP_DISABLED    0
#define PULL_UP_ENABLED     1

#define DEFAULT_STATE_LOW   0
#define DEFAULT_STATE_HIGH  1

/*! \brief The pin used for the button on the
 * [Zumo Shield for Arduino](http://www.pololu.com/product/2504).
 *
 * This does not really belong here in this general pushbutton library and will
 * probably be removed in the future. */
#define ZUMO_BUTTON 12

class PushbuttonStateMachine
{
public:
  PushbuttonStateMachine();
  bool getSingleDebouncedRisingEdge(bool value);
private:
  uint8_t state;
  uint16_t prevTimeMillis;
};

class PushbuttonBase
{
public:

  // wait for button to be pressed, released, or pressed and released
  void waitForPress();
  void waitForRelease();
  void waitForButton();

  // more complex functions that return true once for each button transition
  // from released to pressed or pressed to released
  bool getSingleDebouncedPress();
  bool getSingleDebouncedRelease();

  // indicates whether button is currently pressed
  virtual bool isPressed() = 0;

private:

  PushbuttonStateMachine pressState;
  PushbuttonStateMachine releaseState;
};

/* \class Pushbutton Pushbutton.h
 * \brief Interface with pushbuttons on dedicated pins.
 *
 */
class Pushbutton : public PushbuttonBase
{
public:

  // constructor; takes arguments specifying whether to enable internal pull-up
  // and the default state of the pin that the button is connected to
  Pushbutton(uint8_t pin, uint8_t pullUp = PULL_UP_ENABLED,
      uint8_t defaultState = DEFAULT_STATE_HIGH);

  void init()
  {
    if (!initialized)
    {
      initialized = true;
      init2();
    }
  }

  void init2();

  virtual bool isPressed();

private:

  bool initialized;
  uint8_t _pin;
  bool _pullUp;
  bool _defaultState;
};

#endif
