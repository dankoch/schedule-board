#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include "sys/stat.h"

#include "util/file_utils.h"

namespace util {
  int removeDirectory(std::string path) {
    return rmdir(path.c_str());
	}

  int createDirectory(std::string path) {
    return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }

  int clearDirectory(std::string path) {
    removeDirectory(path);

    return createDirectory(path);
  }

  size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);

    return written;
  }

  void copyFile(std::string source, std::string destination) {
    std::ifstream  src(source, std::ios::binary);
    std::ofstream  dst(destination, std::ios::binary);

    dst << src.rdbuf();
  }

  std::string getFileContents(const std::string filename) {
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);

    if (in)
    {
      std::string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();

      return(contents);
    }

    throw(errno);
  }
}