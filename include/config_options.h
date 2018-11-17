#ifndef CONFIG_OPTIONS_H
#define CONFIG_OPTIONS_H

#include <stdlib.h>
#include <string>

#include "cxxopts.hpp"

class ConfigOptions {

  public:
    ConfigOptions(int argc, char **argv);

    std::string getStoragePath();
    std::string getLogPath();
    std::string getLogLevel();
};

#endif