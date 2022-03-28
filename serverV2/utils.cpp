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

bool	isValidPrefix(std::string *s)
{
	size_t	len;
	size_t	i;

	if ((*s)[0] != '/')
		(*s).insert(0, "/");
	len = (*s).length();
	for (i = 0 ; (*s)[i] ; i++)
	{
		if (std::isprint((*s)[i]) == false)
			return (false);
		if ((*s)[i] == '/'
				&& i != len - 1
				&& (*s)[i + 1] == '/')
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

bool	isValidDomainName(std::string s)
{
	size_t	len;
	size_t	i;

	if (s[0] == '.' || s[0] == '-')
		return (false);
	len = s.length();
	if (len > 67)
		return (false);
	for (i = 0 ; s[i] ; i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 32;
	}
	for (i = 0 ; s[i] ; i++)
	{
		if ((s[i] < 'a' || s[i] > 'z')
				&& std::isdigit(s[i] == false)
				&& s[i] != '.'
				&& s[i] != '-')
			return (false);
		if (s[i] == '-' && i != 0
				&& i != len - 1
				&& (s[i + 1] == '-' || s[i + 1] == '.'))
			return (false);
		if (s[i] == '.' && i != 0
				&& i != len - 1
				&& (s[i + 1] == '.' || s[i + 1] == '-'))
			return (false);
	}
	return (true);
}
