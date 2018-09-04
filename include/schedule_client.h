#ifndef SCHEDULE_CLIENT_H
#define SCHEDULE_CLIENT_H

#include <stdlib.h>
#include <vector>
#include <string>

#include "calendar_event.h"

class ScheduleClient {

  public:
    virtual vector<CalendarEvent> getLoadedCalendarEvents() {
      return getCalendarEvents(30);
    }

    virtual vector<CalendarEvent> getLoadedCalendarEvents(int dayRange);

    virtual void fetchCalendarData(int dayRange);
};

#endif