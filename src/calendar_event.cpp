#include <stdlib.h>
#include <string>

#include "calendar_event.h"

CalendarEvent::CalendarEvent(std::string name, int year, int month, int day, int hour, int minute) {
  setName(name);
  setTime(year, month, day, hour, minute);
}

void CalendarEvent::setName (std::string n) {
  name = n;
}

void CalendarEvent::setTime (int y, int mon, int d, int h, int min) {
  year = y;
  month = mon;
  day = d;
  hour = h;
  min = min;
}

char* CalendarEvent::getName() {
  return name;
}

int CalendarEvent::getYear() {
  return year;
}

int CalendarEvent::getMonth() {
  return month;
}

int CalendarEvent::getDay() {
  return day;
}

int CalendarEvent::getHour() {
  return hour;
}

int CalendarEvent::getMinute() {
  return minute;
}