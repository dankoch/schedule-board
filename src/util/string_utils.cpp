#include <stdlib.h>
#include <string>
#include <iostream>

#include "util/string_utils.h"

namespace util {
  std::string trim(std::string stringToTrim) {
    size_t place = stringToTrim.find_first_not_of(" \t");
    stringToTrim.erase(0, place);

    place = stringToTrim.find_last_not_of(" \t");
    if (std::string::npos != place)
      stringToTrim.erase(place+1);

    return stringToTrim;
	}
}