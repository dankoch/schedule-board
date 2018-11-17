#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <time.h>
#include <curl/curl.h>
#include <algorithm>

#include "spdlog/spdlog.h"
#include "gumbo.h"

#include "config_options.h"
#include "calendar_event.h"
#include "file_utils.h"
#include "string_utils.h"

#include "schedule_client.h"

class CentralCinemaScheduleClient : ScheduleClient {
  const std::string TB_MONTH_EVENT_TABLE_CELL_CLASS = "futureday";
  const std::string TB_DAYOFMONTH_CLASS = "dayofmonth-num";
  const std::string TB_EVENTTITLE_CLASS = "event-title";
  const std::string TB_EVENTTIME_CLASS = "event-time";

  const std::string HTML_CLASS_ATTRIBUTE = "class";

  const std::string URL_TEMPLATE = "https://public.ticketbiscuit.com/CentralCinema/Calendar/%d/%d";
  const std::string FILE_SUBPATH = "/central";
  const std::string LOCAL_EVENT_CACHE_FILE = "central_events.json";
  const std::string LOCAL_EVENT_DOWNLOAD_LOCATION = "/tmp";

  shared_ptr<ConfigOptions> _options;
  std::shared_ptr<spdlog::logger> _logger;

  std::vector<CalendarEvent> loadedCalendarEvents;

  struct MonthRequest {
    int month,
    int year,
    std::string filePath
  };

  struct FetchedDataMetadata {

  }

public:
  CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options) : _options(options) {
    _logger = spdlog::get("schedule_board_logger");
  }

  vector<CalendarEvent> fetchCalendarData(std::tm* startDate, std::tm* endDate) {
    std::vector<CalendarEvent> fetchedEvents;

    /* Determine which buckets need to be queried. */
    std::vector<MonthRequest> monthsToQuery;

    for (int yearIndex = startDate->tm_year; yearIndex <= endDate->tm_year; yearIndex++) {
      if (yearIndex != startDate->tm_year && yearIndex != endDate->tm_year) {
        /* if current year doesn't match the start or end year, we need to query the entire year. */
        for (int monthIndex = 0; monthIndex < 12; monthIndex++) {
          monthsToQuery.push_back(MonthRequest(monthIndex, yearIndex, ""));
        }
      } else if (yearIndex != endDate->tm_month) {
        /* the startDate year isn't this year, but the endDate is.  Add all the months from start of year until endDate. */
        for (int monthIndex = monthIndex->tm_month; monthIndex <= 11; monthIndex++) {
          monthsToQuery.push_back(MonthRequest(monthIndex, yearIndex, ""));
        }
      } else if (yearIndex != startDate->tm_year) {
        /* the endDate year isn't this year, but the startDate is.  Add all the months through EOY. */
        for (int monthIndex = 0; monthIndex <= endDate->tm_month; monthIndex++) {
          monthsToQuery.push_back(MonthRequest(monthIndex, yearIndex, ""));
        }
      }
    }

    /* Create a space to download the collector data. */
    std::string downloadDir = baseDirectory() + LOCAL_EVENT_DOWNLOAD_LOCATION;
    util::clearDirectory(downloadDir);

    /* Download the collector data for each bucket */
    std::vector<MonthRequest> downloadedMonths;
    for(auto const bucket : monthsToQuery) {
      MonthRequest monthExtract;

      if (downloadTicketBiscuitCalendarExtract(downloadDir, bucket, monthExtract) == 0 && !monthExtract.filePath.empty()) {
        downloadedMonths.push_back(monthExtract);
      }
    }

    /* Extract the events from our files and overwrite our data file.*/
    for (auto const monthExtract : downloadedMonths) {
      parseTicketBiscuitCalendarExtract(monthExtract, fetchedEvents);
    }

    return fetchedEvents;
  }

private:

  int downloadTicketBiscuitCalendarExtract(std::string dir, MonthRequest bucket, MonthRequest* monthExtract) {
    int returnCode = -1;

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
        size_t len = strlen(errbuf);

        if(len)
          _logger->error("Failed making request to: {}, {}", requestURL, errbuf);
        else
          _logger->error("Failed making request to: {}, {}", requestURL, curl_easy_strerror(res));
      } else {
        monthExtract = new MonthRequest(bucket.month, bucket.year, requestDestination);
        returnCode = 0;
      }

      curl_easy_cleanup(curl_handle);
      curl_global_cleanup();
    }

    return returnCode;
  }

  std::string buildTicketBiscuitRequestURL(MonthRequest bucket) {
    char buffer[75];

    sprintf(buffer, URL_TEMPLATE.c_str(), bucket.year, bucket.month);
    return std::string(buffer);
  }


  int parseTicketBiscuitCalendarExtract(MonthRequest extract, std::vector<CalendarEvent>& fetchedEvents) {
    int returnCode = 0;

    GumboOutput* output = gumbo_parse(util::getFileContents(extract.filePath).str_c());
    traverseTicketBiscuiltCalendarHtmlTree(output->root, extract, fetchedEvents);

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return returnCode;
  }

  void traverseTicketBiscuitCalendarHtmlTree(GumboNode* node, const MonthRequest& extract, 
                                                            std::vector<CalendarEvent>& fetchedEvents) {

    /* We've reached a terminus.  Break the recursion. */
    if (node->type != GUMBO_NODE_ELEMENT) {
      return;
    }

    /* Check to see if this is a 'futureday' table cell.  If so, parse the cell as a discrete event. */
    GumboAttribute* classList;
    if ((classList = gumbo_get_attribute(&node->v.element.attributes, HTML_CLASS_ATTRIBUTE))) {
      if (strcmp(classList->value, TB_MONTH_EVENT_TABLE_CELL_CLASS) == 0) {
        CalendarEvent eventEntry;
        eventEntry.setYear(extract.year);
        eventEntry.setMonth(extract.month);

        traverseTicketBiscuitEventCell(node, eventEntry);
        fetchedEvents.push_back(eventEntry);        
      }
    }

    /* Otherwise, keep recursing. */
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      traverseTicketBiscuitCalendarHtmlTree(static_cast<GumboNode*>(children->data[i]));
    }
  };

  void traverseTicketBiscuitEventCell(GumboNode* node, CalendarEvent& eventEntry) {

    /* Check to see if this node has any of the HTML classes that indicate it contains a key schedule text. */
    GumboAttribute* classList;
    if ((classList = gumbo_get_attribute(&node->v.element.attributes, HTML_CLASS_ATTRIBUTE))) {
      if (strcmp(classList->value, TB_DAYOFMONTH_CLASS) == 0) {
        eventEntry.setDayOfMonth(std::stoi(getNodeText(node)));       
      } else if (strcmp(classList->value, TB_EVENTTITLE_CLASS) == 0) {
        eventEntry.setName(getNodeText(node));         
      } else if (strcmp(classList->value, TB_EVENTTIME_CLASS) == 0) {
        std::tm timeOfDay;
        strptime(getNodeText(node).c_str(), "%I:%M %p", &timeOfDay);

        eventEntry.setHour(timeOfDay.tm_hour);
        eventEntry.setMinute(timeOfDay.tm_min);      
      }
    }

    /* Otherwise, keep recursing. */
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      traverseTicketBiscuitEventCell(static_cast<GumboNode*>(children->data[i]));
    }    
  }

  std::string baseDirectory() {
    return _options->getStoragePath() + FILE_SUBPATH;
  }

  std::string eventCacheFilePath() {
    return baseDirectory() + LOCAL_EVENT_CACHE_FILE;
  }
};

static std::string getNodeText(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  }

  if (node->type == GUMBO_NODE_ELEMENT) {
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      std::string nodeText = getNodeText(static_cast<GumboNode*>(children->data[i]));

      if (!nodeText.empty()) {
        return util::trim(nodeText);
      }
    }
  }

  return "";
}