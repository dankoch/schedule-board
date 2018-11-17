#ifndef SCHEDULE_CLIENT_H
#define SCHEDULE_CLIENT_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>

#include "calendar_event.h"

class ScheduleClient {

  public:

    /** Retrieve calendar data from the source associated with this client.
    *
    * The schedule board with call this method to pull the list of calendar events for this source between the 
    * specified start and end date.
    *
    * @param startDate Start date for the calendar data to fetch.
    * @param endDate End date for the calendar data to fetch.
    */
    virtual vector<CalendarEvent> fetchCalendarData(std::tm* startDate, std::tm* endDate);
};

#endif