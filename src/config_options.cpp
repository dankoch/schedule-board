#include <stdlib.h>
#include <string>

#include "cxxopts.hpp"

#include "config_options.h"

const std::string DEFAULT_STORAGE_PATH = "/tmp"

ConfigOptions::ConfigOptions(int argc, char **argv) {

  cxxopts::Options options(argv[0], "Program to pull event schedules and show it on the LED Matrix.");

  options
    .allow_unrecognised_options()
    .add_options()
    ("local-file-storage", 
      "Location to store extracted schedule files.", 
      cxxopts::value<std::string>(storagePath) -> default_value(DEFAULT_STORAGE_PATH));

  auto result = options.parse(argc, argv);
}

std::string ConfigOptions::getStoragePath() {
  return storagePath;
}