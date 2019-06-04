/*
 * LongTicker.h - Library to extend Ticker for longer times
 * Created by Walter D. Martins, April 25, 2019
 */
#ifndef LONG_TICKER_H
#define LONG_TICKER_H

#include <Ticker.h>

class LongTicker 
{
public:
  static const int MAX_MINUTES_TICKER = 60;
  // Keep ticker and cb public
  Ticker ticker;
  Ticker::callback_t cb;

public:
  LongTicker();
  LongTicker(std::string tickerName);
  ~LongTicker();
    
  void once(int minutes, Ticker::callback_t cb);
  void showStatus(const char *functionName);
  bool isRunning(void) {
    return ticker.active();
  }
  void detach() {
    ticker.detach();
  }
  void setTotalMinutesLeft(int minutes) {
    totalMinutesLeft = minutes;
  }

  int getTotalMinutesLeft(void) {
    return totalMinutesLeft;
  }

private:
  int totalMinutesLeft;
  std::string tickerName;
};

#endif // LONG_TICKER_H