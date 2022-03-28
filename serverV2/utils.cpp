# include "webserv.hpp"

bool	isNumber(std::string s)
{
	for (int i = 0 ; s[i] ; i++)
	{
		if (std::isdigit(s[i]) == false)
			return (false);
	}
	return (true);
}

bool	isValidPort(std::string s)
{
	size_t	len;

	len = s.length();
	if (len > 5 || (len == 5 && s.compare("65535") > 0))
		return (false);
	return (true);
}
