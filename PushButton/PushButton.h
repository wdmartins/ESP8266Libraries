/*
 * PushButton.h - Library to handle different pressing times of a push button
 * Created by Walter D. Martins, May 15, 2019
 */

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <time.h>

class PushButton {
  public:
    PushButton(int gpio, float shortPressSeconds, float longPressSeconds):
      _gpio(gpio),
      _shortPressSeconds(shortPressSeconds),
      _longPressSeconds(longPressSeconds),
      _onPressedOnStart(nullptr),
      _onVeryShortPress(nullptr),
      _onShortPress(nullptr),
      _onLongPress(nullptr) {
          pinMode(gpio, INPUT);
      }

    void setShortPressCB(void (*func)()) {
      _onShortPress = func;
    }

    void setLongPressCB(void (*func)()) {
      _onLongPress = func;
    }

    void setup(void (*pressedOnStartCb)(), void (*veryShortCb)(), void (*shortCb)(), void (*longCb)()) {
      _onPressedOnStart = pressedOnStartCb;
      _onVeryShortPress = veryShortCb;
      _onShortPress = shortCb;
      _onLongPress = longCb;
      _currentValue = digitalRead(_gpio);
      Serial.print("[PUSHBUTTON] Setup. Current value: "); Serial.println(_currentValue);
      if(_currentValue == LOW && _onPressedOnStart != nullptr) {
        Serial.println("PressButton was LOW on start");
        _onPressedOnStart();
      }
    }

    void run() {
      int newState = digitalRead(_gpio);
      if (newState != _currentValue) {
        Serial.print("[PUSHBUTTON]: State change. New state: "); Serial.println(newState);
        _currentValue = newState;
        if (newState == LOW) {
          _pressTime = _getCurrentTimeRaw();
        } else {
          time_t now = _getCurrentTimeRaw();
          int timePressed = now - _pressTime;
          Serial.print("[PUSHBUTTON]: Time button was pressed: "); Serial.print(timePressed); Serial.println(" seconds");
          if (timePressed <= _shortPressSeconds) {
            Serial.println("[PUSHBUTTON]: Very Short Press");
            if (_onVeryShortPress != nullptr) {
              _onVeryShortPress();            
            }
          } else if (timePressed > _shortPressSeconds && timePressed <= _longPressSeconds) {
            Serial.println("[PUSHBUTTON]: Short Press");
            if (_onShortPress != nullptr) {
              _onShortPress();            
            }
          } else {
            Serial.println("[PUSHBUTTON]: Long Press");
            if (_onLongPress != nullptr) {
              _onLongPress();
            }
          }
          _pressTime = -1;
        }
      }
    }

  private:
    time_t _getCurrentTimeRaw() {
        time(nullptr);
        delay(1000);
        return time(nullptr);
    }
    int _gpio;
    float _shortPressSeconds;
    float _longPressSeconds;
    int _currentValue;
    void (*_onPressedOnStart)();
    void (*_onVeryShortPress)();
    void (*_onShortPress)();
    void (*_onLongPress)();
    time_t _pressTime;

};
#endif PUSHBUTTON_H