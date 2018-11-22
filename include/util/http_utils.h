#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "spdlog/spdlog.h"

namespace util {

  /** Make an HTTP GET call against a URL and download the results to a local path.
  *
  *
  * @param url Start date for the calendar data to fetch.
  * @param endDate End date for the calendar data to fetch.
  */
  int httpGetToFile(std::string url, std::string destinationPath);
}

#endif