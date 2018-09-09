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
}