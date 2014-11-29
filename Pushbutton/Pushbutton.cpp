#include "Pushbutton.h"

void PushbuttonStateMachine::PushbuttonStateMachine()
{
  state = 0;
}

// state 0: The value is considered to be true.
// state 1: The value was considered to be true, but there
//   was a recent false reading so it might be falling.
// state 2: The value is considered to be false.
// state 3: The value was considered to be false, but there
//   was a recent true reading so it might be rising.
//
// The transition from state 3 to state 0 is the point where we
// have successfully detected a rising edge an we return true.
//
// The prevTimeMillis variable holds the last time that we
// transitioned to states 1 or 3.
bool PushbuttonStateMachine::getSingleDebouncedRisingEdge(bool value)
{
  uint16_t timeMillis = millis();

  switch (state)
  {
  case 0:
    // If value is false, proceed to the next state.
    if (!value)
    {
      prevTimeMillis = timeMillis;
      state = 1;
    }
    break;

  case 1:
    if (value)
    {
      // The value is true or bouncing, so go back to previous (initial)
      // state.
      state = 0;
    }
    else if ((uint16_t)(timeMillis - prevTimeMillis) >= 15)
    {
      // It has been at least 15 ms and the value is still false, so
      // proceed to the next state.
      state = 2;
    }
    break;

  case 2:
    // If the value is true, proceed to the next state.
    if (value)
    {
      prevTimeMillis = timeMillis;
      state = 3;
    }
    break;

  case 3:
    if (!value)
    {
      // The value is false or bouncing, so go back to previous state.
      state = 2;
    }
    else if ((uint16_t)(timeMillis - prevTimeMillis) >= 15)
    {
      // It has been at least 15 ms and the value is still true, so
      // go back to the initial state and report this rising edge.
      state = 0;
      return true;
    }
    break;
  }

  return false;
}

PushbuttonBase::PushbuttonBase()
{
  initialized = false;
}

// wait for button to be pressed
void PushbuttonBase::waitForPress()
{
  init(); // initialize if necessary

  do
  {
    while (!_isPressed()); // wait for button to be pressed
    delay(10);            // debounce the button press
  }
  while (!_isPressed());   // if button isn't still pressed, loop
}

// wait for button to be released
void PushbuttonBase::waitForRelease()
{
  init(); // initialize if necessary

  do
  {
    while (_isPressed()); // wait for button to be released
    delay(10);           // debounce the button release
  }
  while (_isPressed());   // if button isn't still released, loop
}

// wait for button to be pressed, then released
void PushbuttonBase::waitForButton()
{
  waitForPress();
  waitForRelease();
}

// indicates whether button is pressed
bool PushbuttonBase::isPressed()
{
  init(); // initialize if necessary

  return _isPressed();
}

// Uses a finite state machine to detect a single button press and returns
// true to indicate the press (false otherwise).  It requires the button to be
// released for at least 15 ms and then pressed for at least 15 ms before
// reporting the press.  This function handles all necessary debouncing and
// should be called repeatedly in a loop.
bool PushbuttonBase::getSingleDebouncedPress()
{
  return pressState.getSingleDebouncedRisingEdge(isPressed());
}

// Uses a finite state machine to detect a single button release and returns
// true to indicate the release (false otherwise).  It requires the button to be
// pressed for at least 15 ms and then released for at least 15 ms before
// reporting the release.  This function handles all necessary debouncing and
// should be called repeatedly in a loop.
bool PushbuttonBase::getSingleDebouncedRelease()
{
  return pressState.getSingleDebouncedRisingEdge(!isPressed());
}

// constructor; takes arguments specifying whether to enable internal pull-up
// and the default state of the pin that the button is connected to
Pushbutton::Pushbutton(uint8_t pin, uint8_t pullUp, uint8_t defaultState)
{
  _pin = pin;
  _pullUp = pullUp;
  _defaultState = defaultState;
}

// initializes I/O pin for use as button inputs
void Pushbutton::init2()
{
  if (_pullUp == PULL_UP_ENABLED)
  {
    pinMode(_pin, INPUT_PULLUP);
  }
  else
  {
    pinMode(_pin, INPUT); // high impedance
  }

  delayMicroseconds(5); // give pull-up time to stabilize
}

// button is pressed if pin state differs from default state
inline bool Pushbutton::_isPressed()
{
  return (digitalRead(_pin) == LOW) ^ (_defaultState == DEFAULT_STATE_LOW);
}
