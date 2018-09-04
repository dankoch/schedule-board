#include <stdlib.h>

#include "config_options.h"

int main(int argc, char** argv) {
  std::shared_ptr<ConfigOptions> options(new ConfigOptions(argc, argv));

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