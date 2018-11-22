#include <stdlib.h>
#include <string>
#include <ctime>

#include <nlohmann/json.hpp>
using nlohmann::json;

#include "calendar_event.h"

CalendarEvent::CalendarEvent() {};
CalendarEvent::CalendarEvent(std::string name, std::tm time) : name(name), time(time) {};

std::string CalendarEvent::getName() const {
  return name;
}

std::tm CalendarEvent::getTime() const {
  return time;
}

void CalendarEvent::setName(std::string name) {
  name = name;
}

void CalendarEvent::setCalendarYear(int year) {
  time.tm_year = year - 1900;
}

void CalendarEvent::setMonth(int month) {
  time.tm_mon = month;
}

void CalendarEvent::setDayOfMonth(int day) {
  time.tm_mday = day;
}

void CalendarEvent::setHour(int hour) {
  time.tm_hour = hour;
}

void CalendarEvent::setMinute(int minute) {
  time.tm_min = minute;
}

void CalendarEvent::setTime(std::tm newTime) {
  time = newTime;
}

void to_json(json& j, const CalendarEvent& event) {
  char buffer[25];

  std::tm eventTime = event.getTime();
  strftime(buffer, 25, "%FT%TZ", &eventTime);

  j = json{
           {"name", event.getName()}, 
           {"time", buffer}
          };
}

void from_json(const json& j, CalendarEvent& event) {
    event.setName(j.at("name").get<std::string>());

    std::tm time;
    strptime(j.at("time").get<std::string>().c_str(), "%FT%TZ", &time);

    event.setTime(time);
}