#include <Arduino.h>
#include "LongTicker.h"

void repeat(LongTicker *myTicker) {
  myTicker->showStatus("repeat");
  int tickerMinutes = std::min(myTicker->getTotalMinutesLeft(), LongTicker::MAX_MINUTES_TICKER);
  if (tickerMinutes <= 0) {
    myTicker->cb();
  } else {
    if (myTicker->getTotalMinutesLeft() > LongTicker::MAX_MINUTES_TICKER) {
      myTicker->setTotalMinutesLeft(myTicker->getTotalMinutesLeft() - tickerMinutes);
      myTicker->ticker.once(LongTicker::MAX_MINUTES_TICKER * 60, repeat, myTicker);
      return;
    }
    myTicker->ticker.once(myTicker->getTotalMinutesLeft() * 60, myTicker->cb);
    myTicker->setTotalMinutesLeft(0);
  }
}

LongTicker::LongTicker()
: totalMinutesLeft(0) {}

LongTicker::LongTicker(std::string tickName)
: totalMinutesLeft(0)
, tickerName(tickName) {}

LongTicker::~LongTicker() {
  ticker.detach();
}

void LongTicker::showStatus(const char *functionName) {
  Serial.printf("[TICKER]: func(%s), ticker(%s), minutesLeft(%d)\n",
    functionName, this->tickerName.c_str(), this->totalMinutesLeft);  
}

void LongTicker::once(uint16_t minutes, Ticker::callback_t cb) {
  if (ticker.active()) {
    Serial.println("T[TICKER]: his ticker is already running. It will be detached");
    ticker.detach();
  }
  this->totalMinutesLeft = minutes;
  this->showStatus("once");
  if (minutes > MAX_MINUTES_TICKER) {
    this->totalMinutesLeft = minutes - MAX_MINUTES_TICKER;
    this->cb = cb;
    ticker.once(MAX_MINUTES_TICKER * 60, repeat, this);
    return;
  }
  ticker.once(minutes * 60, cb);
  this->totalMinutesLeft = 0;
}
