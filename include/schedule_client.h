#ifndef SCHEDULE_CLIENT_H
#define SCHEDULE_CLIENT_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>

#include "calendar_event.h"

class ScheduleClient {

  public:
    virtual vector<CalendarEvent> getCalendarEvents(std::tm* startDate, std::tm* endDate);
};

#endif