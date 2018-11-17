#include <stdlib.h>
#include <string>
#include <ctime>

#include <nlohmann/json.hpp>
using nlohmann::json;

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
    time.tm_year = year - 1900;
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

  void setTime(std::tm newTime) {
    time = newTime;
  }

  void to_json(json& j, const CalendarEvent& event) {
    char buffer[25];

    j = json{
             {"name", event.getName()}, 
             {"time", strftime(buffer, 25, "%FT%TZ", event.getTime())}
            };
  }

  void from_json(const json& j, CalendarEvent& event) {
      event.setName(j.at("name").get<std::string>());

      char buffer[40];
      event.setTime(strptime(buffer, "%FT%TZ", j.at("time").get<std::string>()));
  }

}