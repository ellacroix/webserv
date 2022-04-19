# include "webserv.hpp"

bool	isNumber(std::string s)
{
	if (s.empty() == true)
		return (false);
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

	//	ADD LEADING '/' IF MISSING
	if ((*s)[0] != '/')
		(*s).insert(0, "/");
	if (s->find("//") != std::string::npos)
		return (false);
	len = (*s).length();
	for (i = 0 ; (*s)[i] ; i++)
	{
		if (std::isprint((*s)[i]) == false)
			return (false);
	}
	//	ADD TRAILING '/' IF MISSING
	if ((*s)[len - 1] != '/')
		s->push_back('/');
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

bool	areValidDomainNames(std::vector<std::string> & v)
{
	std::vector<std::string>::const_iterator	it;
	std::vector<std::string>::const_iterator	ite;

	it = v.begin();
	it++;
	ite = v.end();
	while (it != ite)
	{
		if (isValidDomainName(*it) == false)
			return (false);
		it++;
	}
	return (true);
}

bool	isValidClientMaxBodySize(std::string & s)
{
	size_t		i;

	i = 0;
	while (std::isdigit(s[i]))
		i++;
	if (i == 0)
		return (false);
	if (s[i] == 'K' || s[i] == 'M')
		s[i] += 32;
	if (s[i] != '\0' && s[i] != 'k' && s[i] != 'm')
		return (false);
	if ((s[i] == 'k' || s[i] == 'm') && s[i + 1] != '\0')
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

bool	isValidAbsolutePathToFile(std::string & s)
{
//	if (isValidPrefix(&s) == false)
//		return (false);
	if (s[0] != '/'
			|| s[s.length() - 1] == '/'
			|| s.find("//") != std::string::npos)
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
	if (isValidAbsolutePathToFile(v[size - 1]) == false)
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

bool	areValidIndexes(std::vector<std::string> & v)
{
	size_t	i;
	size_t	size;

	i = 1;
	size = v.size();
	while (i < size)
	{
		if (isValidIndex(v[i]) == false)
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
	if (isSupportedHttpRedirCode(v[1]))
//			&& isValidAbsolutePathToFile(v[2]))
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

bool	isValidReqUri(std::string const & s)
{
	return (s[0] == '/'
			&& s.find("/../") == std::string::npos
			&& s.find("//") == std::string::npos);
	//Manage absoluteURI ? GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1
}

std::string	findUriExtension(std::string uri)
{
	std::string										extension;
	std::string										file;
	size_t											last_pt;
	size_t											last_slash;

	last_slash = uri.find_last_of("/");
	file = uri.substr(last_slash + 1, uri.length() - last_slash);
	//std::cout << "=== FILE ONLY\t=\t\"" << file << "\"" << std::endl;

	last_pt = file.find_last_of(".");
	if (last_pt == std::string::npos)
	{
		//std::cout << "=== EXT ONLY\t=\tEMPTY" << std::endl;
		return (extension);
	}
	extension = file.substr(last_pt + 1, file.length() - last_pt);
	//std::cout << "=== EXT ONLY\t=\t\"" << extension << "\"" << std::endl;
	return (extension);
}

std::string numberToString(int nb)
{
    std::ostringstream oss;

    oss << nb;
    return oss.str();
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}

void	logger(std::string message)
{
	static bool 			first = true;
	static pthread_mutex_t	mutex;
	static std::ofstream	file;
	//static unsigned long	start_time;
	static pid_t			main;

	//struct timeval	timestamp;

	//gettimeofday(&timestamp, NULL);

	if (first == true)
	{
		first = false;
		pthread_mutex_init(&mutex, NULL);
		file.open("log.log", std::ios::app);
		//start_time = 1000000 * timestamp.tv_sec + timestamp.tv_usec;
		main = syscall(__NR_gettid);
	}

	//unsigned long	time = 1000000 * timestamp.tv_sec + timestamp.tv_usec - start_time;

	pid_t x = syscall(__NR_gettid);

	if (x - main == 0)
	{
		pthread_mutex_lock(&mutex);
		file << "[" << currentDateTime() << "][MAIN]\t " << message << std::endl;
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		pthread_mutex_lock(&mutex);
		file << "[" << currentDateTime() << "][" << x - main << "]\t\t " << message << std::endl;
		pthread_mutex_unlock(&mutex);
	}
}	
