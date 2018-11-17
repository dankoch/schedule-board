#include <stdlib.h>

#include "spdlog/spdlog.h"

#include "file_utils.h"
#include "config_options.h"

int main(int argc, char** argv) {
  std::shared_ptr<ConfigOptions> options(new ConfigOptions(argc, argv));

  util::createDirectory(options.getLogPath());
  auto _logger = spd::rotating_logger_mt("schedule_board_logger", options.getLogPath() + "/schedule_board", 1048576 * 50, 4);
  _logger->set_level(spdlog::level::from_str(options.getLogLevel()));

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput* output = gumbo_parse(contents.c_str());
  getEvents(output->root);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}