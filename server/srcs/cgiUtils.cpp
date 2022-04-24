#include "webserv.hpp"

char	*ft_strdup(const char *src)
{
	char *dst;
	dst = new char[strlen(src) + 1];
	dst = strcpy(dst, src);
	return (dst);
}

char	*ft_strdup(std::string src)
{
	char *dst;
	dst = new char[src.length() + 1];
	dst = strcpy(dst, src.c_str());
	return (dst);
}

void	deleteArray(char **array)
{
	for (int i = 0; array[i]; i++)
		delete [] array[i];
	delete [] array;
}
