#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace util {

	/* Trims leading and trailing whitespace from stringToTrim. */
  int trim(std::string stringToTrim);
}