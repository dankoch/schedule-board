#ifndef CENTRAL_CINEMA_SCHEDULE_CLIENT_H
#define CENTRAL_CINEMA_SCHEDULE_CLIENT_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>

#include "calendar_event.h"
#include "schedule_client.h"

class CentralCinemaScheduleClient : public ScheduleClient {

  public:
    CentralCinemaScheduleClient::CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options);
    CentralCinemaScheduleClient::~CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options);

    /* Function used to fetch the client data.  Set as a functor to enable mocking / testing. */
    std::function<int(std::string, std::string)> httpGetToFile;

    /* Implementation of ScheduleClient interface method. */
    vector<CalendarEvent> fetchCalendarData(std::tm* startDate, std::tm* endDate);
};

#endif