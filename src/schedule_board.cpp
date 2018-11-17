#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <time.h>
#include <errno.h>

#include "spdlog/spdlog.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

#include "schedule_board.h"

class ScheduleBoard {
  const std::string FILE_SUBPATH = "/main";
  const std::string LOCAL_EVENT_CACHE_FILE = "event_cache.json";

public:
  ScheduleBoard(std::shared_ptr<ConfigOptions> options) : _options(options) {
    _logger = spdlog::get("schedule_board_logger");
  }

  int startBoard() {
    //TODO
  }

private:

  int writeLocalEventCacheFile(const std::vector<CalendarEvent> &fetchedEvents) {
    /* Overwrites the current local cache file with the contents in fetchedEvents. */

    json j;
    j["events"] = fetchedEvents;

    std::ofstream outFile(eventCacheFilePath());
    outFile << j;
    outFile.close();

    if (!outFile) {
      _logger->error("Failed to write local event cache file. Error: {}", strerror(errno));
      return -1;
    }

    return 0;
  }

  std::vector<CalendarEvent> loadLocalEventCacheFile() {
    /* Fetches the local catch file and returns it. */

    std::vector<CalendarEvent> loadedEvents;
    json j;

    std::ifstream inFile(eventCacheFilePath());
    if (inFile >> j) {
      if (j.count("events") > 0) {
        loadedEvents = j["events"];
      }
    } else {
      _logger->error("Failed to read local event cache file. Error: {}", strerror(errno));
    }
    inFile.close();

    return loadedEvents;
  }
}