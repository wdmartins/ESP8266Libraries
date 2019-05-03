#include <Arduino.h>
#include "LongTicker.h"

void repeat(LongTicker *myTicker) {
  myTicker->showStatus("Begin repeat");
  int tickerMinutes = std::min(myTicker->getTotalMinutesLeft(), LongTicker::MAX_MINUTES_TICKER);
  if (tickerMinutes <= 0) {
    myTicker->cb();
  } else {
    if (myTicker->getTotalMinutesLeft() > LongTicker::MAX_MINUTES_TICKER) {
      myTicker->setTotalMinutesLeft(myTicker->getTotalMinutesLeft() - tickerMinutes);
      myTicker->ticker.once(LongTicker::MAX_MINUTES_TICKER * 60, repeat, myTicker);
      myTicker->showStatus("End repeat");
      return;
    }
    myTicker->ticker.once(myTicker->getTotalMinutesLeft() * 60, myTicker->cb);
    myTicker->setTotalMinutesLeft(0);
  }
  myTicker->showStatus("End repeat");
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
  Serial.print("[TICKER] "); Serial.print("("); Serial.print(functionName); Serial.print(") ");
  Serial.print(": Name: "); Serial.print(this->tickerName.c_str()); Serial.print(", minutesLeft: "); Serial.println(this->totalMinutesLeft);  
}

void LongTicker::once(int minutes, Ticker::callback_t cb) {
  if (ticker.active()) {
    Serial.println("This ticker is already running. It will be detached");
    ticker.detach();
  }
  this->totalMinutesLeft = minutes;
  this->showStatus("Begin once");
  if (minutes > MAX_MINUTES_TICKER) {
    this->totalMinutesLeft = minutes - MAX_MINUTES_TICKER;
    this->cb = cb;
    ticker.once(MAX_MINUTES_TICKER * 60, repeat, this);
    this->showStatus("End once");
    return;
  }
  ticker.once(minutes * 60, cb);
  this->totalMinutesLeft = 0;
  this->showStatus("End once");
}
