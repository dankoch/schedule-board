#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <time.h>
#include <errno.h>
#include <fstream>

#include "spdlog/spdlog.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

#include "config_options.h"

#include "schedule_board.h"

/********************************************************************
   LOCAL CONSTANTS AND DECLARATIONS
********************************************************************/

namespace {
  const std::string FILE_SUBPATH = "/main";
  const std::string LOCAL_EVENT_CACHE_FILE = "event_cache.json";
}

/********************************************************************
   PUBLIC METHOD IMPLEMENTATIONS
********************************************************************/


ScheduleBoard::ScheduleBoard(std::shared_ptr<ConfigOptions> options) : _options(options) {
  _logger = spdlog::get("schedule_board_logger");
}

int ScheduleBoard::startBoard() {
  return 0;
}

/********************************************************************
   PRIVATE METHOD IMPLEMENTATIONS
********************************************************************/

int ScheduleBoard::writeLocalEventCacheFile(const std::vector<CalendarEvent> &fetchedEvents) {
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

std::vector<CalendarEvent> ScheduleBoard::loadLocalEventCacheFile() {
  /* Fetches the local catch file and returns it. */

  std::vector<CalendarEvent> loadedEvents;
  json j;

  std::ifstream inFile(eventCacheFilePath());
  if (inFile >> j) {
    if (j.count("events") > 0) {
      j["events"].get_to(loadedEvents);
    }
  } else {
    _logger->error("Failed to read local event cache file. Error: {}", strerror(errno));
  }
  inFile.close();

  return loadedEvents;
}

std::string ScheduleBoard::eventCacheFilePath() {
  return _options->getStoragePath() + FILE_SUBPATH + "/" + LOCAL_EVENT_CACHE_FILE;
}