#include <StatusLED.h>

void statusCb(StatusLED *led) {
  int state = digitalRead(led->getGpio());
  digitalWrite(led->getGpio(), !state);
}

void StatusLED::setStatus(Status status) {
  if (status != state) {
    ticker.detach();
    float period = 0;
    state = status;
    switch(state) {
      case initializing:
        period = INIT;
        break;
      case stable:
        period = STABLE;
        break;
      case no_wifi:
        period = NO_WIFI;
        break;
     }
     if(period == 0) {
       digitalWrite(gpio, HIGH);
     } else {
       ticker.attach(period, statusCb, this);
     }
  }
}