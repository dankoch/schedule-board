#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <time.h>
#include <algorithm>

#include "spdlog/spdlog.h"
#include "gumbo.h"

#include "config_options.h"
#include "calendar_event.h"
#include "util/file_utils.h"
#include "util/string_utils.h"
#include "util/http_utils.h"

#include "client/schedule_client.h"
#include "client/central_cinema_schedule_client.h"

/********************************************************************
   LOCAL CONSTANTS AND DECLARATIONS
********************************************************************/

namespace {
  const std::string TB_MONTH_EVENT_TABLE_CELL_CLASS = "futureday";
  const std::string TB_DAYOFMONTH_CLASS = "dayofmonth-num";
  const std::string TB_EVENTTITLE_CLASS = "event-title";
  const std::string TB_EVENTTIME_CLASS = "event-time";

  const std::string HTML_CLASS_ATTRIBUTE = "class";

  const std::string URL_TEMPLATE = "https://public.ticketbiscuit.com/CentralCinema/Calendar/%d/%d";
  const std::string FILE_SUBPATH = "/central";
  const std::string LOCAL_EVENT_CACHE_FILE = "central_events.json";
  const std::string LOCAL_EVENT_DOWNLOAD_LOCATION = "/tmp";

  struct MonthRequest {
    int month;
    int year;
    std::string filePath;
  };

  int downloadTicketBiscuitCalendarExtract(std::string dir, MonthRequest bucket, MonthRequest* monthExtract);
  std::string buildTicketBiscuitRequestURL(MonthRequest bucket);
  int parseTicketBiscuitCalendarExtract(MonthRequest extract, std::vector<CalendarEvent>& fetchedEvents);
  void traverseTicketBiscuitCalendarHtmlTree(GumboNode* node, const MonthRequest& extract, 
                                                            std::vector<CalendarEvent>& fetchedEvents);
  void traverseTicketBiscuitEventCell(GumboNode* node, CalendarEvent& eventEntry);
  std::string getNodeText(GumboNode *node);
}

/********************************************************************
   PUBLIC METHOD IMPLEMENTATIONS
********************************************************************/

CentralCinemaScheduleClient::CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options) : _options(options) {
  _logger = spdlog::get("schedule_board_logger");

  // Set the HTTP Fetch method to be via the util cURL wrapper call (to be overridden for mocking/testing)
  httpGetToFile = util::httpGetToFile;
}

std::vector<CalendarEvent> CentralCinemaScheduleClient::fetchCalendarData(std::tm* startDate, std::tm* endDate) {
  std::vector<CalendarEvent> fetchedEvents;

  /* Determine which buckets need to be queried. */
  std::vector<MonthRequest> monthsToQuery;

  for (int yearIndex = startDate->tm_year; yearIndex <= endDate->tm_year; yearIndex++) {
    /* Start by assuming we're querying all months for the given year. */
    int monthIndexMin = 0;
    int monthIndexMax = 11;

    /* Check if given year is the same as the query start year or end year. 
       If so, limit the months queried to that start/end boundary. */
    if (yearIndex == startDate->tm_year) { monthIndexMin = startDate->tm_year; }
    if (yearIndex == endDate->tm_year) { monthIndexMax = endDate->tm_year; }

    for (int monthIndex = monthIndexMin; monthIndex <= monthIndexMax; monthIndex++) {
      monthsToQuery.push_back(MonthRequest{monthIndex, yearIndex, ""});
    }
  }

  /* Create a space to download the collector data. */
  std::string downloadDir = _options->getStoragePath() + FILE_SUBPATH + LOCAL_EVENT_DOWNLOAD_LOCATION;
  util::clearDirectory(downloadDir);

  /* Download the collector data for each bucket */
  std::vector<MonthRequest> downloadedMonths;
  for(auto const bucket : monthsToQuery) {
    MonthRequest monthExtract;

    if (downloadTicketBiscuitCalendarExtract(downloadDir, bucket, &monthExtract) == 0 && !monthExtract.filePath.empty()) {
      downloadedMonths.push_back(monthExtract);
    }
  }

  /* Extract the events from our files and overwrite our data file.*/
  for (auto const monthExtract : downloadedMonths) {
    parseTicketBiscuitCalendarExtract(monthExtract, fetchedEvents);
  }

  return fetchedEvents;
}

CentralCinemaScheduleClient::~CentralCinemaScheduleClient() {

}

/********************************************************************
   LOCAL METHOD IMPLEMENTATIONS
********************************************************************/

namespace {
  int downloadTicketBiscuitCalendarExtract(std::string dir, MonthRequest bucket, MonthRequest* monthExtract) {

    /* Ensure the target directory for the download extract exists. */
    util::createDirectory(dir);

    /* Build the URL and target directory. */
    std::string requestURL = buildTicketBiscuitRequestURL(bucket);
    std::string requestDestination = dir + "/" + std::to_string(bucket.year) + "_" + std::to_string(bucket.month + 1) + ".html";

    /* Make the request.  If it succeeds – set the MonthExtract with the necessary information and return. */
    if (util::httpGetToFile(requestURL, requestDestination) == 0) {
      *monthExtract = {bucket.month, bucket.year, requestDestination};      

      return 0;
    }

    /* Didn't work out.*/
    return -1;
  }

  std::string buildTicketBiscuitRequestURL(MonthRequest bucket) {
    char buffer[75];

    sprintf(buffer, URL_TEMPLATE.c_str(), bucket.year, bucket.month + 1);
    return std::string(buffer);
  }


  int parseTicketBiscuitCalendarExtract(MonthRequest extract, std::vector<CalendarEvent>& fetchedEvents) {
    int returnCode = 0;

    GumboOutput* output = gumbo_parse(util::getFileContents(extract.filePath).c_str());
    traverseTicketBiscuitCalendarHtmlTree(output->root, extract, fetchedEvents);

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
    if ((classList = gumbo_get_attribute(&node->v.element.attributes, HTML_CLASS_ATTRIBUTE.c_str()))) {
      if (strcmp(classList->value, TB_MONTH_EVENT_TABLE_CELL_CLASS.c_str()) == 0) {
        CalendarEvent eventEntry;
        eventEntry.setCalendarYear(extract.year);
        eventEntry.setMonth(extract.month);

        traverseTicketBiscuitEventCell(node, eventEntry);
        fetchedEvents.push_back(eventEntry);        
      }
    }

    /* Otherwise, keep recursing. */
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      traverseTicketBiscuitCalendarHtmlTree(static_cast<GumboNode*>(children->data[i]), extract, fetchedEvents);
    }
  };

  void traverseTicketBiscuitEventCell(GumboNode* node, CalendarEvent& eventEntry) {

    /* Check to see if this node has any of the HTML classes that indicate it contains a key schedule text. */
    GumboAttribute* classList;
    if ((classList = gumbo_get_attribute(&node->v.element.attributes, HTML_CLASS_ATTRIBUTE.c_str()))) {
      if (strcmp(classList->value, TB_DAYOFMONTH_CLASS.c_str()) == 0) {
        eventEntry.setDayOfMonth(std::stoi(getNodeText(node)));       
      } else if (strcmp(classList->value, TB_EVENTTITLE_CLASS.c_str()) == 0) {
        eventEntry.setName(getNodeText(node));         
      } else if (strcmp(classList->value, TB_EVENTTIME_CLASS.c_str()) == 0) {
        std::tm timeOfDay;
        strptime(getNodeText(node).c_str(), "%I:%M %p", &timeOfDay);

        eventEntry.setHour(timeOfDay.tm_hour);
        eventEntry.setMinute(timeOfDay.tm_min);      
      }
    }

    /* Otherwise, keep recursing. */
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      traverseTicketBiscuitEventCell(static_cast<GumboNode*>(children->data[i]), eventEntry);
    }    
  }

  std::string getNodeText(GumboNode *node) {
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
}