#include "webserv.hpp"

int		checkPath(std::string &path, int permission)
{
	struct stat fs;
	
	printf("Checking %s\n",path.c_str() + 1);

	if (stat(path.c_str() + 1, &fs) != 0)
		return DOES_NOT_EXIST;
	
	if (permission == READ)
	{
		if (!(fs.st_mode & S_IRUSR))
			return FORBIDDEN;
	}
	else if (permission == WRITE)
	{
		if (!(fs.st_mode & S_IWUSR))
			return FORBIDDEN;
	}
	else if (permission == EXECUTE)
	{
		if (!(fs.st_mode & S_IXUSR))
			return FORBIDDEN;
	}
	
	if (S_ISDIR(fs.st_mode))
		return IS_A_DIRECTORY;
	else if (S_ISREG(fs.st_mode))
		return IS_A_FILE;

	return -1;
}