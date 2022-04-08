#include "Location.hpp"

// CONSTRUCTORS / DESTRUCTOR

Location::Location(void) :
	_autoIndex(false),
	_returnCode(-1),
//	_clientMaxBodySize(0),
	_rootIsSet(false),
//	_errorPageIsSet(false),
	_autoIndexIsSet(false),
	_indexIsSet(false),
	_returnIsSet(false),
	_limitExceptIsSet(false)//,
//	_clientMaxBodySizeIsSet(false)
{
	return ;
}

Location::~Location(void)
{
	return ;
}

Location::Location(Location const & src) :
	_prefix(src._prefix),
	_root(src._root),
//	_errorPage(src._errorPage),
	_autoIndex(src._autoIndex),
	_index(src._index),
	_limitExcept(src._limitExcept),
	_returnCode(src._returnCode),
	_returnUri(src._returnUri),
//	_clientMaxBodySize(src._clientMaxBodySize),
	_rootIsSet(src._rootIsSet),
//	_errorPageIsSet(src._errorPageIsSet),
	_autoIndexIsSet(src._autoIndexIsSet),
	_indexIsSet(src._indexIsSet),
	_returnIsSet(src._returnIsSet),
	_limitExceptIsSet(src._limitExceptIsSet)//,
//	_clientMaxBodySizeIsSet(src._clientMaxBodySizeIsSet)
{
	return ;
}

Location &	Location::operator=(Location const & rhs)
{
	this->_prefix = rhs._prefix;
	this->_root = rhs._root;
//	this->_errorPage = rhs._errorPage;
	this->_autoIndex = rhs._autoIndex;
	this->_index = rhs._index;
	this->_limitExcept = rhs._limitExcept;
	this->_returnCode = rhs._returnCode;
	this->_returnUri = rhs._returnUri;
//	this->_clientMaxBodySize = rhs._clientMaxBodySize;
	this->_rootIsSet = rhs._rootIsSet;
//	this->_errorPageIsSet = rhs._errorPageIsSet;
	this->_autoIndexIsSet = rhs._autoIndexIsSet;
	this->_indexIsSet = rhs._indexIsSet;
	this->_returnIsSet = rhs._returnIsSet;
	this->_limitExceptIsSet = rhs._limitExceptIsSet;
//	this->_clientMaxBodySizeIsSet = rhs._clientMaxBodySizeIsSet;
	return (*this);
}

//	SETTERS
void	Location::setPrefix(std::string s)
{
	this->_prefix = s;
}

void	Location::setRoot(std::string s)
{
	this->_root = s;
}

void	Location::setAutoindex(bool b)
{
	this->_autoIndex = b;
}

//void	Location::setIndex(std::vector<std::string> v)
//void	Location::setIndex(std::string s)
//{
//	//	this->_index = std::vector<std::string>(v);
//	this->_index = s;
//}

void	Location::setIndex(std::vector<std::string> v)
{
	size_t	i;
	size_t	size;

	i = 1;
	size = v.size();
	while (i < size)
	{
		this->_index.push_back(v[i]);
		i++;
	}
	// OR REPLACE WHOLE VECTOR ?
}

void	Location::setReturnCode(int	n)
{
	this->_returnCode = n;
}

void	Location::setReturnUri(std::string s)
{
	this->_returnUri = s;
}

void	Location::setLimitExcept(std::vector<std::string> v)
{
	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite;

	it = v.begin() + 1;
	ite = v.end();
	while (it != ite)
	{
		this->_limitExcept.push_back(*it);
		it++;
	}
}

/*
void	Location::setClientMaxBodySize(std::string s)
{
	char	*endPtr;

	this->_clientMaxBodySize = std::strtoul(s.c_str(), &endPtr, 10);
	if (*endPtr == 'k')
		this->_clientMaxBodySize *= K;
	if (*endPtr == 'm')
		this->_clientMaxBodySize *= M;
}
*/

//	GETTERS
/*
std::map<int, std::string> &	Location::getErrorPage(void)
{
	return (this->_errorPage);
}
*/

std::string						Location::getPrefix(void) const
{
	return (this->_prefix);
}

int								Location::getReturnUri(void) const
{
	return (this->_returnCode);
}

std::string						Location::getReturnUri(void) const
{
	return (this->_returnUri);
}

//	UTILITIES
void	Location::reset(void)
{
	this->_prefix = "";
	this->_root = "";
//	this->_errorPage = std::map<int, std::string>();
	this->_autoIndex = false;
//	this->_index = "";
	this->_index = std::vector<std::string>();
	this->_limitExcept = std::vector<std::string>();
	this->_returnCode = -1;
	this->_returnUri = "";
//	this->_clientMaxBodySize = 0;

	this->_rootIsSet = false;
//	this->_errorPageIsSet = false;
	this->_autoIndexIsSet = false;
	this->_indexIsSet = false;
	this->_returnIsSet = false;
	this->_limitExceptIsSet = false;
//	this->_clientMaxBodySizeIsSet = false;
}

void	Location::display(void) const
{
//	std::map<int, std::string>::const_iterator	map_it;
//	std::map<int, std::string>::const_iterator	map_ite;
	std::vector<std::string>::const_iterator	vct_it;
	std::vector<std::string>::const_iterator	vct_ite;
	int											i;

	std::cout << "\t\t_prefix\t\t\t=\t\"" << this->_prefix << "\"" << std::endl;
	std::cout << "\t\t_root\t\t\t=\t\"" << this->_root << "\"" << std::endl;
	/*
	if (this->_errorPage.empty() == true)
		std::cout << "\t\t_errorPage[ ]\t\t=\tempty" << std::endl;
	else
	{
		map_it = this->_errorPage.begin();
		map_ite = this->_errorPage.end();
		while (map_it != map_ite)
		{
			std::cout << "\t\t_errorPage[" << (*map_it).first << "]\t\t=\t\""
				<< (*map_it).second << "\"" << std::endl;
			map_it++;
		}
	}
	*/
	std::cout << std::boolalpha;
	std::cout << "\t\t_autoIndex\t\t=\t" << this->_autoIndex << std::endl;
	if (this->_index.empty() == true)
		std::cout << "\t\t_index[ ]\t\t=\tempty" << std::endl;
	else
	{
		vct_it = this->_index.begin();
		vct_ite = this->_index.end();
		i = 0;
		while (vct_it != vct_ite)
		{
			std::cout << "\t\t_index[" << i << "]\t\t=\t\""
				<< *vct_it << "\"" << std::endl;
			vct_it++;
			i++;
		}
	}
	if (this->_limitExcept.empty() == true)
		std::cout << "\t\t_limitExcept[ ]\t\t=\tempty" << std::endl;
	else
	{
		vct_it = this->_limitExcept.begin();
		vct_ite = this->_limitExcept.end();
		i = 0;
		while (vct_it != vct_ite)
		{
			std::cout << "\t\t_limitExcept[" << i << "]\t\t=\t\""
				<< *vct_it << "\"" << std::endl;
			vct_it++;
			i++;
		}
	}
	std::cout << "\t\t_return\t\t\t=\t" << this->_returnCode << std::endl;
	std::cout << "\t\t_returnUri\t\t=\t\"" << this->_returnUri << "\"" << std::endl;
//	std::cout << "\t\t_clientMaxBodySize\t=\t"
//		<< this->_clientMaxBodySize << std::endl;


	std::cout << "\t\t_rootIsSet\t\t=\t" << this->_rootIsSet << std::endl;
//	std::cout << "\t\t_errorPageIsSet\t\t=\t" << this->_errorPageIsSet << std::endl;
	std::cout << "\t\t_autoIndexIsSet\t\t=\t" << this->_autoIndexIsSet << std::endl;
	std::cout << "\t\t_indexIsSet\t\t=\t" << this->_indexIsSet << std::endl;
	std::cout << "\t\t_returnIsSet\t\t=\t" << this->_returnIsSet << std::endl;
	std::cout << "\t\t_limitExceptIsSet\t=\t" << this->_limitExceptIsSet << std::endl;
//	std::cout << "\t\t_clientMaxBodySizeIsSet\t=\t"
//		<< this->_clientMaxBodySizeIsSet << std::endl;
}

Location *	Location::clone(void) const
{
	Location *ptr;

	ptr = new Location(*this);
	return (ptr);
}

bool	Location::validate(void) const
{
	return (this->_rootIsSet || this->_returnIsSet);
}

void	Location::resetIsDefBooleans(void)
{
	this->_rootIsSet = false;
//	this->_errorPageIsSet = false;
	this->_autoIndexIsSet = false;
	this->_indexIsSet = false;
	this->_returnIsSet = false;
	this->_limitExceptIsSet = false;
//	this->_clientMaxBodySizeIsSet = false;
}
