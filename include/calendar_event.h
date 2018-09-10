#ifndef CALENDAR_EVENT_H
#define CALENDAR_EVENT_H

#include <stdlib.h>
#include <string>
#include <ctime>

class CalendarEvent {
  std::string name;
  std::tm time;

  /* Sort events according to their time. */
  friend bool operator< (const X& lhs, const X& rhs){ return (mktime(&(lhs.getTime())) < mktime(&(rhs.getTime()))) };
  friend bool operator> (const X& lhs, const X& rhs){ return rhs < lhs; }
  friend bool operator<=(const X& lhs, const X& rhs){ return !(lhs > rhs); }
  friend bool operator>=(const X& lhs, const X& rhs){ return !(lhs < rhs); }

  public:
    CalendarEvent(std::string name, std::tm time);

    std::string getName();
    std::tm getTime();

    void setName(std::string name);
    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setHour(int hour);
    void setMinute(int minute);
};

#endif