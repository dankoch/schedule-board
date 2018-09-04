#ifndef CALENDAR_EVENT_H
#define CALENDAR_EVENT_H

#include <stdlib.h>

class CalendarEvent {
  char* name;
  int year, month, day, hour, minute;

  public:
    CalendarEvent(std::string name, int year, int month, int day, int hour, int minute);

    void setName (std::string name);
    void setTime (int year, int month, int day, int hour, int minute);

    std::string getName();
    int   getMinute();
    int   getHour();
    int   getDay();
    int   getMonth();
    int   getYear();
};

#endif