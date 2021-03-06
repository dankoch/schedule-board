#include <stdlib.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "util/file_utils.h"
#include "config_options.h"

int main(int argc, char** argv) {

  /* Parse the configuration options.*/
  std::shared_ptr<ConfigOptions> options(new ConfigOptions(argc, argv));

  /* Set up the logger for the Schedule Board to use, including the log directory. */
  util::createDirectory(options->getLogPath());
  auto _logger = spdlog::rotating_logger_mt("schedule_board_logger", options->getLogPath() + "/schedule_board", 1048576 * 50, 4);
  _logger->set_level(spdlog::level::from_str(options->getLogLevel()));

  /* Fire up the Schedule Board. */
  
}