#include <TimeUtils.h>

time_t TimeUtils::getCurrentTimeRaw() {
  // Get time twice to avoid ....1970
  time(nullptr);
  delay(1000);
  return time(nullptr);
}

struct tm *TimeUtils::getCurrentTime() {
  time_t now = getCurrentTimeRaw();
  return localtime(&now);
}

int TimeUtils::minutesTillMidnight(struct tm time) {
  return ((23 - time.tm_hour) * 60) + (time.tm_min == 0 ? 0 : 60 - time.tm_min);
}

int TimeUtils::minutesTillMidnight() {
    return(minutesTillMidnight(*getCurrentTime()));
}

std::string TimeUtils::getTimeStr(struct tm *time) {
  char buffer[50];
  strftime(buffer, 20, "%c", time);
  return std::string(buffer);
}

std::string TimeUtils::getTimeStr(time_t time) {
    return getTimeStr(localtime(&time));
}