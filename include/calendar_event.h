#ifndef CALENDAR_EVENT_H
#define CALENDAR_EVENT_H

#include <stdlib.h>
#include <string>
#include <ctime>

#include <nlohmann/json.hpp>
using nlohmann::json;

class CalendarEvent {
  std::string name;
  std::tm time;

  /* Sort events according to their time. */
  friend bool operator< (const CalendarEvent& lhs, const CalendarEvent& rhs) {
    std::tm lhsTime = lhs.getTime();
    std::tm rhsTime = rhs.getTime();

    return (mktime(&lhsTime) < mktime(&rhsTime)); 
  };
  friend bool operator> (const CalendarEvent& lhs, const CalendarEvent& rhs){ return rhs < lhs; }
  friend bool operator<=(const CalendarEvent& lhs, const CalendarEvent& rhs){ return !(lhs > rhs); }
  friend bool operator>=(const CalendarEvent& lhs, const CalendarEvent& rhs){ return !(lhs < rhs); }

  public:
    CalendarEvent();
    CalendarEvent(std::string name, std::tm time);

    std::string getName() const;
    std::tm getTime() const;

    void setName(std::string name);
    void setCalendarYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setDayOfMonth(int day);
    void setHour(int hour);
    void setMinute(int minute);

    void setTime(std::tm newTime);
};

void to_json(json& j, const CalendarEvent& event);
void from_json(const json& j, CalendarEvent& event);

#endif