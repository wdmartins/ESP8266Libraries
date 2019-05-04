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
    const float STABLE = 0;
    const float CUSTOM_1 = 0.1;
    const float INIT = 0.5;
    const float CUSTOM_2 = 1;
    const float NO_WIFI = 3;
    const float CUSTOM_3 = 6;


    enum Status {
      unknown,
      initializing,
      stable,
      no_wifi,
      custom_1,
      custom_2,
      custom_3
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
