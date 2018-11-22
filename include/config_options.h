#ifndef CONFIG_OPTIONS_H
#define CONFIG_OPTIONS_H

#include <stdlib.h>
#include <string>

#include "cxxopts.hpp"

class ConfigOptions {

private:
  std::string storagePath;
  std::string logPath;
  std::string logLevel;

public:  /* Used to initialize an 'all-default' version of ConfigOptions.  For use in testing.*/
  ConfigOptions();

  /* Used to initialize ConfigOptions from command line arguments. */
  ConfigOptions(int argc, char **argv);

  std::string getStoragePath() const;
  void setStoragePath(std::string path);

  std::string getLogPath() const;
  void setLogPath(std::string logPath);

  std::string getLogLevel() const;
  void setLogLevel(std::string logLevel);

};

#endif