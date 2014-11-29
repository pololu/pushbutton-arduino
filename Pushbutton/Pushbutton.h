/*! \file Pushbutton.h
 *
 * See the Pushbutton class reference for more information about this library.
 *
 * \class Pushbutton Pushbutton.h
 * \brief Read button presses and releases with debouncing
 *
 */

#ifndef Pushbutton_h
#define Pushbutton_h

#include <Arduino.h>

// This ZUMO_BUTTON definition is for the Zumo Shield
#define ZUMO_BUTTON 12

#define PULL_UP_DISABLED    0
#define PULL_UP_ENABLED     1

#define DEFAULT_STATE_LOW   0
#define DEFAULT_STATE_HIGH  1

class PushbuttonBase
{
public:
    PushbuttonBase();

    // wait for button to be pressed, released, or pressed and released
    void waitForPress();
    void waitForRelease();
    void waitForButton();

    // more complex functions that return true once for each button transition
    // from released to pressed or pressed to released
    bool getSingleDebouncedPress();
    bool getSingleDebouncedRelease();

    // indicates whether button is currently pressed
    bool isPressed();

    // Initializes I/O pin for use as button input.
    // This gets called automatically so normally you will not need to call this,
    // but it might be useful if the pin for the button is in the wrong state.
    virtual void init2() = 0;

protected:

    virtual bool _isPressed() = 0;

private:

    uint8_t gsdpState;
    uint8_t gsdrState;
    uint16_t gsdpPrevTimeMillis;
    uint16_t gsdrPrevTimeMillis;
    bool initialized;

    inline void init()
    {
      if (!initialized)
      {
        initialized = true;
        init2();
      }
    }
};

class Pushbutton : public PushbuttonBase
{
public:
    // constructor; takes arguments specifying whether to enable internal pull-up
    // and the default state of the pin that the button is connected to
    Pushbutton(uint8_t pin, uint8_t pullUp = PULL_UP_ENABLED, uint8_t defaultState = DEFAULT_STATE_HIGH);

    virtual void init2();

protected:
    virtual bool _isPressed();

private:
    uint8_t _pin;
    bool _pullUp;
    bool _defaultState;
};

#endif
