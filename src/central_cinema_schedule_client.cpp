#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <curl/curl.h>

#include "spdlog/spdlog.h"
#include "gumbo.h"

#include "config_options.h"
#include "calendar_event.h"
#include "file_utils.h"

#include "schedule_client.h"

class CentralCinemaScheduleClient : ScheduleClient {
  const std::string EVENT_NAME_CLASS = "event-title";
  const std::string CLASS_ATTRIBUTE = "class";

  const std::string URL_TEMPLATE = "https://public.ticketbiscuit.com/CentralCinema/Calendar/%d/%d";
  const std::string FILE_SUBPATH = "/central";
  const std::string LOCAL_EVENT_CACHE_FILE = "central_events.json";
  const std::string LOCAL_EVENT_DOWNLOAD_LOCATION = "/tmp";

  shared_ptr<ConfigOptions> _options;
  std::shared_ptr<spdlog::logger> _logger;

  std::vector<CalendarEvent> loadedCalendarEvents;

  struct MonthBucket {
    int month,
    int year
  };

public:
  CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options) : _options(options) {
    _logger = spdlog::get("schedule_client_logger");
  }

  vector<CalendarEvent> getCalendarEvents(std::tm* startDate, std::tm* endDate) {

  }

private:

  void fetchCalendarData(std::tm* startDate, std::tm* endDate); {
    std::vector<CalendarEvent> fetchedEvents;

    /* Determine which buckets need to be queried. */
    std::vector<MonthBucket> monthsToQuery;

    for (int yearIndex = startDate->tm_year; yearIndex <= endDate->tm_year; yearIndex++) {
      if (yearIndex != startDate->tm_year && yearIndex != endDate->tm_year) {
        /* if current year doesn't match the start or end year, we need to query the entire year. */
        for (int monthIndex = 0; monthIndex < 12; monthIndex++) {
          monthsToQuery.push_back(MonthBucket(monthIndex, yearIndex));
        }
      } else if (yearIndex != endDate->tm_month) {
        /* the startDate year isn't this year, but the endDate is.  Add all the months from start of year until endDate. */
        for (int monthIndex = monthIndex->tm_month; monthIndex <= 11; monthIndex++) {
          monthsToQuery.push_back(MonthBucket(monthIndex, yearIndex));
        }
      } else if (yearIndex != startDate->tm_year) {
        /* the endDate year isn't this year, but the startDate is.  Add all the months through EOY. */
        for (int monthIndex = 0; monthIndex <= endDate->tm_month; monthIndex++) {
          monthsToQuery.push_back(MonthBucket(monthIndex, yearIndex));
        }
      }
    }

    /* Create a space to download the collector data. */
    std::string downloadDir = baseDirectory() + LOCAL_EVENT_DOWNLOAD_LOCATION;
    util::clearDirectory(downloadDir);

    /* Download the collector data for each bucket */
    std::vector<std::string> extractPaths;
    for(auto const bucket : monthsToQuery) {

      std::string extractPath;
      if (downloadTicketBiscuitCalendarExtract(downloadDir, bucket, &extractPath) == 0) {
        extractPaths.push_back(extractPath);
      }
    }

    /* If we succesfully downloaded any files, extract the events from them and overwrite our data file.*/
    if (extractPaths.size() > 0) {
      for (auto const extractPath : extractPaths) {

      }
    }
  }

  int downloadTicketBiscuitCalendarExtract(std::string dir, MonthBucket bucket, std::string& extractPath) {
    int returnCode = 0;

    /* Ensure the target directory for the download extract exists. */
    createStorageDir(dir);

    std::string requestURL = buildTicketBiscuitRequestURL(bucket);
    std::string requestDestination = dir + "/" + to_string(year) + "_" + to_string(month) + ".html";

    CURL *curl;
    CURLcode res;
    char errbuf[CURL_ERROR_SIZE];
 
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, requestURL);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
      errbuf[0] = 0;

      FILE* pagefile = fopen(requestDestination, "wb");
      if(pagefile) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
        res = curl_easy_perform(curl);
        fclose(pagefile);
      }

      _logger->info("Made request to: {} to write output to {}", requestURL, requestDestination);

      if(res != CURLE_OK) {
        returnCode = 1;
        size_t len = strlen(errbuf);

        if(len)
          _logger->error("Failed making request to: {}, {}", requestURL, errbuf);
        else
          _logger->error("Failed making request to: {}, {}", requestURL, curl_easy_strerror(res));
      } else {
        extractPath = requestDestination;
      }

      curl_easy_cleanup(curl_handle);
      curl_global_cleanup();
    }

    return returnCode;
  }

  int parseTicketBiscuitCalendarExtract(std::string extractPath, std::vector<CalendarEvent>& fetchedEvents) {
    int returnCode = 0;

    GumboOutput* output = gumbo_parse(util::getFileContents(extractPath).str_c());
    getEvents(output->root);

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return returnCode;
  }  

  std::string buildTicketBiscuitRequestURL(MonthBucket bucket) {
    char buffer[75];

    sprintf(buffer, URL_TEMPLATE.c_str(), bucket.year, bucket.month);
    return std::string(buffer);
  }

  std::string baseDirectory() {
    return _options->getStoragePath() + FILE_SUBPATH;
  }
};

static const void getText(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    std::cout << node->v.text.text << std::endl;  
  }

  if (node->type == GUMBO_NODE_ELEMENT) {
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      getText(static_cast<GumboNode*>(children->data[i]));
    }
  }
}

static void getEvents(GumboNode* node) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* classList;
  if ((classList = gumbo_get_attribute(&node->v.element.attributes, CLASS_ATTRIBUTE))) {
      if (strcmp(classList->value, EVENT_NAME_CLASS) == 0) {
        getText(node);        
      }
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    getEvents(static_cast<GumboNode*>(children->data[i]));
  }
};

int main(int argc, char** argv) {
  std::shared_ptr<ConfigOptions> options(new ConfigOptions(argc, argv));
  auto console = spdlog::stdout_color_mt("schedule_client_logger");
}