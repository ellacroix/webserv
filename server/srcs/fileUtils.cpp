#include "webserv.hpp"

bool pathExists(std::string path) {
  struct stat fs;
  return (stat(path.c_str(), &fs) == 0);
}

bool isDirectory(std::string path) {
  struct stat fs;
  stat(path.c_str(), &fs);
  return S_ISDIR(fs.st_mode);
}

bool isFile(std::string path) {
  struct stat fs;
  stat(path.c_str(), &fs);
  return S_ISREG(fs.st_mode);
}

bool canRead(std::string path) {
	struct stat fs;
	stat(path.c_str(), &fs);
	return (fs.st_mode & S_IRUSR);
}

bool canWrite(std::string path) {
	struct stat fs;
	stat(path.c_str(), &fs);
	return (fs.st_mode & S_IWUSR);
}

bool canExecute(std::string path) {
	struct stat fs;
	stat(path.c_str(), &fs);
	return (fs.st_mode & S_IXUSR);
}
