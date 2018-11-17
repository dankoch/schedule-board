#include <stdlib.h>
#include <string>
#include <iostream>

#include "string_utils.h"

namespace util {
  int trim(std::string stringToTrim) {
    size_t p = s.find_first_not_of(" \t");
    s.erase(0, p);

    p = s.find_last_not_of(" \t");
    if (string::npos != p)
      s.erase(p+1);
	}
}