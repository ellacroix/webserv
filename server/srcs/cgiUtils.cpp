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

int		getCgiStatusCode(std::string header)
{
	size_t itFind;
	size_t len;
	std::string status_code;

	itFind = header.find("Status");
	if (itFind == std::string::npos)
		return (200);
	itFind+=8;
	len = header.find(" ", itFind) - itFind;
	status_code = header.substr(itFind, len);
	return (atoi(status_code.c_str()));
}
