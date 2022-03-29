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

bool	isValidClientMaxBodySize(std::string s)
{
	size_t		i;

	i = 0;
	while (std::isdigit(s[i]) == true)
		i++;
	if (i == 0)
		return (false);
	if (s[i] == 'K' || s[i] == 'M')
		s[i] += 32;
	if (s[i] != '\0' && s[i] != 'k' && s[i] != 'm')
		return (false);
	if ((s[i] == 'k' || s[i] == 'm') && s[++i] != '\0')
		return (false);
	return (true);
}

bool	isSupportedHttpErrorCode(std::string s)
{
	return (s == "400" || s == "403" || s == "404"
			|| s == "405" || s == "406" || s == "408"
			|| s == "413" || s == "413" || s == "414"
			|| s == "415" || s == "416" || s == "417" 
			|| s == "500" || s == "501" || s == "503" 
			|| s == "505");
}

bool	isValidUri(std::string & s)
{
	if (isValidPrefix(&s) == false)
		return (false);
	if (s[s.length() - 1] == '/')
		return (false);
	return (true);
}

bool	isValidErrorPage(std::vector<std::string> & v)
{
	size_t		i;
	size_t		j;
	size_t		size;
	std::string	uri;

	i = 1;
	size = v.size();
	while (i < size - 1)
	{
		if (isSupportedHttpErrorCode(v[i]) == false)
			return (false);
		j = i + 1;
		while (j < size - 1)
		{
			if (v[i] == v[j])
				return (false);
			j++;
		}
		i++;
	}
	if (isValidUri(v[size - 1]) == false)
		return (false);
	return (true);
}

bool	isValidAutoindex(std::string s)
{
	return (s == "on" || s == "off");
}

bool	isValidIndex(std::string s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
			return (false);
		i++;
	}
	return (true);
}

bool	isSupportedHttpRedirCode(std::string s)
{
	return (s == "301" || s == "302" || s == "304");
}

bool	isValidReturn(std::vector<std::string> & v)
{
	if (isSupportedHttpRedirCode(v[1])
			&& isValidUri(v[2]))
		return (true);
	return (false);
}

bool	isSupportedHttpMethod(std::string s)
{
	return (s == "GET" || s == "POST" || s == "DELETE");
}

bool	isValidLimitExcept(std::vector<std::string> v)
{
	bool	get;
	bool	post;
	bool	del;
	size_t	i;
	size_t	size;

	i = 1;
	size = v.size();
	get = false;
	post = false;
	del = false;
	while (i < size)
	{
		if (isSupportedHttpMethod(v[i]) == false)
			return (false);
		if (v[i] == "GET")
		{
			if (get == true)
				return (false);
			get = true;
		}
		else if (v[i] == "POST")
		{
			if (post == true)
				return (false);
			post = true;
		}
		else if (v[i] == "DELETE")
		{
			if (del == true)
				return (false);
			del = true;
		}
		i++;
	}
	return (true);
}
