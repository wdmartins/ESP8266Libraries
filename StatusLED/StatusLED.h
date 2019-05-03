/*
 * StatusLED.h - Library to abstract Arduino GPIO to show different status of the system using an LED
 * Created by Walter D. Martins, April 25, 2019
 */
#ifndef STATUS_LED_H
#define STATUS_LED_H
#include <Arduino.h>
#include <Ticker.h>

class StatusLED {
  public:
    const float INIT = 0.5;
    const float NO_WIFI = 2;
    const float STABLE = 0;

    enum Status {
      unknown,
      initializing,
      stable,
      no_wifi
    };
  
    StatusLED(int gpioIndex, Status status=initializing)
      :gpio(gpioIndex),
      state(unknown) {
        pinMode(gpioIndex, OUTPUT);
        setStatus(status);
      };
    
    void setStatus(Status status);
    int getGpio() {
      return gpio;
    };
  private:
    Ticker ticker;
    Status state;
    int gpio;
};

#endif STATUS_LED_H
