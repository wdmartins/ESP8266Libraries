/*
 * TimeUtils.h - Library of time functions
 * Created by Walter D. Martins, May 23, 2019
 */
#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <Arduino.h>
#include <time.h>

class TimeUtils {
    public:
        static time_t getCurrentTimeRaw();
        static struct tm *getCurrentTime();
        static std::string getTimeStr(struct tm *time);
        static std::string getTimeStr(time_t time);
        static uint16_t minutesTillMidnight(struct tm time);
        static uint16_t minutesTillMidnight();

    private:
        TimeUtils() {}
};

#endif  // TIME_UTILS_H