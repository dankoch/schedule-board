#include <stdlib.h>
#include <string>
#include <iostream>

#include <curl/curl.h>

#include "gmock/gmock.h"
#include "util/http_utils.h"

namespace util {
  class MockHttpUtil {

   public:
    MOCK_METHOD2(httpGetToFile, int(std::string url, std::string destinationPath));

    
  };
}