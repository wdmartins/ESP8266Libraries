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

uint16_t TimeUtils::minutesTillMidnight(struct tm time) {
  return ((23 - time.tm_hour) * 60) + (time.tm_min == 0 ? 0 : 60 - time.tm_min);
}

uint16_t TimeUtils::minutesTillMidnight() {
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