#ifndef CALENDAR_EVENT_H
#define CALENDAR_EVENT_H

#include <stdlib.h>
#include <string>
#include <ctime>

class CalendarEvent {
  std::string name;
  std::tm time;

  public:
    CalendarEvent(std::string name, std::tm time);

    std::string getName();
    std::tm getTime();
};

#endif