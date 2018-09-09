#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace util {

	/* Deletes the specified directory. */
	int removeDirectory(std::string path);

  /* Creates a directory with 755 permissions. */
  int createDirectory(std::string path);

  /* Clear directory. */
  int clearDirectory(std::string path);

  /* Write data to file from stream. */
  size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);

  /* Read the contents of a given file. */
  std::string getFileContents(const std::string filename);
}