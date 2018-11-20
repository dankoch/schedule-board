#include <stdlib.h>
#include <string>
#include <iostream>

#include <curl/curl.h>

#include "http_utils.h"

namespace util {

 int httpGetToFile(std::string url, std::string destinationPath) {
    auto _logger = spdlog::get("schedule_board_logger");

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
        returnCode = 0;
      }

      curl_easy_cleanup(curl_handle);
      curl_global_cleanup();
    }
  }
}