#include <stdlib.h>
#include <string>
#include <ctime>

#include "calendar_event.h"

class CalendarEvent {

public:
  CalendarEvent(std::string name, std::tm time) : name(name), time(time) {};

  std::string getName() {
    return name;
  }

  std::tm getTime() {
    return time;
  }

  void setName(std::string name) {
    name = name;
  }

  void setCalendarYear(int year) {
    time.tm_year = year - 1970;
  }

  void setMonth(int month) {
    time.tm_mon = month;
  }

  void setDayOfMonth(int day) {
    time.tm_mday = day;
  }

  void setHour(int hour) {
    time.tm_hour = hour;
  }

  void setMinute(int minute) {
    time.tm_min = minute;
  }
}