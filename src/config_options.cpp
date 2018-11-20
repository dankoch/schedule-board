#include <stdlib.h>
#include <string>

#include "cxxopts.hpp"

#include "config_options.h"

const std::string DEFAULT_STORAGE_PATH = "/tmp/schedule_board";
const std::string DEFAULT_LOG_PATH = "/var/log/schedule_board";
const std::string DEFAULT_LOG_LEVEL = "warn";

class ScheduleBoard {
  std::string storagePath;
  std::string logPath;
  std::string logLevel;

public:
  ConfigOptions() {
    setStoragePath(DEFAULT_STORAGE_PATH);
    setLogPath(DEFAULT_LOG_PATH);
    setLogLevel(DEFAULT_LOG_LEVEL);    
  }

  ConfigOptions(int argc, char **argv) {

    cxxopts::Options options(argv[0], "Program to pull event schedules and show it on the LED Matrix.");

    options
      .allow_unrecognised_options()
      .add_options()
      ("local-file-storage", 
        "Location to store extracted schedule files.", 
        cxxopts::value<std::string>(storagePath) -> default_value(DEFAULT_STORAGE_PATH))
      ("log-path",
        "Location to write log files.",
        cxxopts::value<std::string>(logPath) -> default_value(DEFAULT_LOG_PATH))
      ("log-level",
        "Log level of warnings to report on.  Possible values: 'error', 'warning' [default], 'info', 'debug'.",
        cxxopts::value<std::string>(logLevel) -> default_value(DEFAULT_LOG_LEVEL))
      ;

    auto result = options.parse(argc, argv);
  }

  std::string getStoragePath() {
    return storagePath;
  }

  void setStoragePath(std::string path) {
    storagePath = path;
  }

  std::string getLogPath() {
    return logPath;
  }

  void setLogPath(std::string path) {
    logPath = path;
  }

  std::string getLogLevel() {
    return logLevel;
  }

  void setLogLevel(std::string level) {
    logLevel = level;
  }
}