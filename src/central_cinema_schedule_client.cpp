#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <curl/curl.h>

#include "calendar_event.h"
#include "gumbo.h"

class CentralCinemaScheduleClient : ScheduleClient {
  const char* EVENT_NAME_CLASS = "event-title";
  const char* CLASS_ATTRIBUTE = "class";

  const char* URL_TEMPLATE = "https://public.ticketbiscuit.com/CentralCinema/Calendar/%d/%d";
  const char* FILE_SUBPATH = "/central";

  shared_ptr<ConfigOptions> options;

  vector<CalendarEvent> loadedCalendarEvents;

public:
  CentralCinemaScheduleClient(std::shared_ptr<ConfigOptions> options) : options(options) {}

  vector<CalendarEvent> getLoadedCalendarEvents(int dayRange) {

  }

  void fetchCalendarData(int dayRange) {

  }

private:

  bool downloadTicketBiscuitCalendarExtract(std::string dir, int year, int month) {
    /* Ensure the target directory for the download extract exists. */
    createStorageDir(dir);

    CURL *curl;
    CURLcode res;
    char errbuf[CURL_ERROR_SIZE];
 
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, buildRequestURL(year, month));
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
      errbuf[0] = 0;

      FILE* pagefile = fopen(buildExtractPath(dir, year, month), "wb");
      if(pagefile) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
        res = curl_easy_perform(curl);
        fclose(pagefile);
      }

      if(res != CURLE_OK) {
        size_t len = strlen(errbuf);
        fprintf(stderr, "\nlibcurl: (%d) ", res);
        if(len)
          fprintf(stderr, "%s%s", errbuf,
                  ((errbuf[len - 1] != '\n') ? "\n" : ""));
        else
          fprintf(stderr, "%s\n", curl_easy_strerror(res));
      }

      curl_easy_cleanup(curl_handle);
      curl_global_cleanup();
    }
  }

  std::string buildRequestURL(int year, int month) {
    char buffer[75];

    sprintf(buffer, URL_TEMPLATE, year, month);
    return std::string(buffer);
  }

  std::string buildExtractPath(std::string dir, int year, int month) {
    return dir + "/" + to_string(year) + "/" + to_string(month) + ".html";
  }

  const int createStorageDir(std::string downloadFolder) {
    return mkdir( options->getStoragePath() + FILE_SUBPATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
};

static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

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