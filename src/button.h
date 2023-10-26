#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <Arduino.h>

const int LONG_PRESS_DURATION = 1000;
const int DEBOUNCE_DELAY = 50;

enum class ButtonState {
    Pressed,
    LongPress,
    Released
};

using Callback = std::function<void(ButtonState)>;

struct Button {
    int _pin;
    ButtonState _state;
    unsigned long _lastPress, _lastDebounce;
    Callback _callback;
    void init(int pin, Callback callback) {
        _pin = pin;
        _lastPress = 0;
        _state = ButtonState::Released;
        _callback = callback;
    }
    void update() {
        ButtonState nextState = state();
        if (nextState != _state) {
            _lastDebounce = millis();
        }
        if (millis() - _lastDebounce < DEBOUNCE_DELAY) {
            if (nextState != _state) {
                _callback(nextState);
            }
        }
        _state = nextState;
    }
    ButtonState state() {
        if (digitalRead(_pin) == HIGH) {
            if (_lastPress == 0) {
                _lastPress = millis();
                return ButtonState::Pressed;
            }
            else if (millis() - _lastPress > LONG_PRESS_DURATION) {
                return ButtonState::LongPress;
            }
        }
        else {
            if (_lastPress != 0) {
                _lastPress = 0;
                return ButtonState::Released;
            }
        }
        return ButtonState::Released;
    }
};

#endif // BUTTON_H