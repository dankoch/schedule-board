#ifndef CENTRAL_CINEMA_SCHEDULE_CLIENT_H
#define CENTRAL_CINEMA_SCHEDULE_CLIENT_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>

#include "config_options.h"
#include "calendar_event.h"
#include "schedule_client.h"

class CentralCinemaScheduleClient : public ScheduleClient {

public:
  CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options);
  ~CentralCinemaScheduleClient();

  /* Function used to fetch the client data.  Set as a functor to enable mocking / testing. */
  std::function<int(std::string, std::string)> httpGetToFile;

  /* Implementation of ScheduleClient interface method. */
  std::vector<CalendarEvent> fetchCalendarData(std::tm* startDate, std::tm* endDate);

private:
  std::shared_ptr<ConfigOptions> _options;
  std::shared_ptr<spdlog::logger> _logger;
};

#endif