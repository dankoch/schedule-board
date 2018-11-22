#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "config_options.h"
#include "calendar_event.h"

class ScheduleBoard {

public:
  ScheduleBoard(std::shared_ptr<ConfigOptions> options);
  int startBoard();

private:
  std::shared_ptr<ConfigOptions> _options;
  std::shared_ptr<spdlog::logger> _logger;

  int writeLocalEventCacheFile(const std::vector<CalendarEvent> &fetchedEvents);
  std::vector<CalendarEvent> loadLocalEventCacheFile();
  std::string eventCacheFilePath();
};