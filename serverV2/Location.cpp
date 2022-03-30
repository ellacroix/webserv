#include "Location.hpp"

// CONSTRUCTORS / DESTRUCTOR

Location::Location(void) :
	_autoIndex(false),
	_returnCode(-1),
	_rootIsSet(false),
	_errorPageIsSet(false),
	_autoIndexIsSet(false),
	_indexIsSet(false),
	_returnIsSet(false),
	_limitExceptIsSet(false)
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
	_errorPage(src._errorPage),
	_autoIndex(src._autoIndex),
	_index(src._index),
	_limitExcept(src._limitExcept),
	_returnCode(src._returnCode),
	_returnUri(src._returnUri),
	_rootIsSet(src._rootIsSet),
	_errorPageIsSet(src._errorPageIsSet),
	_autoIndexIsSet(src._autoIndexIsSet),
	_indexIsSet(src._indexIsSet),
	_returnIsSet(src._returnIsSet),
	_limitExceptIsSet(src._limitExceptIsSet)
{
	return ;
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
void	Location::setIndex(std::string s)
{
//	this->_index = std::vector<std::string>(v);
	this->_index = s;
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

//	GETTERS
std::map<int, std::string> &	Location::getErrorPage(void)
{
	return (this->_errorPage);
}

std::string						Location::getPrefix(void) const
{
	return (this->_prefix);
}

//	UTILITIES
void	Location::reset(void)
{
	this->_prefix = "";
	this->_root = "";
	this->_errorPage = std::map<int, std::string>();
	this->_autoIndex = false;
	this->_index = "";
	this->_limitExcept = std::vector<std::string>();
	this->_returnCode = -1;
	this->_returnUri = "";

	this->_rootIsSet = false;
	this->_errorPageIsSet = false;
	this->_autoIndexIsSet = false;
	this->_indexIsSet = false;
	this->_returnIsSet = false;
	this->_limitExceptIsSet = false;
}

void	Location::display(void) const
{
	std::map<int, std::string>::const_iterator	map_it;
	std::map<int, std::string>::const_iterator	map_ite;
	std::vector<std::string>::const_iterator	vct_it;
	std::vector<std::string>::const_iterator	vct_ite;
	int											i;

	std::cout << "\t\t_prefix\t\t\t=\t\"" << this->_prefix << "\"" << std::endl;
	std::cout << "\t\t_root\t\t\t=\t\"" << this->_root << "\"" << std::endl;
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
	std::cout << std::boolalpha;
	std::cout << "\t\t_autoIndex\t\t=\t" << this->_autoIndex << std::endl;
	std::cout << "\t\t_index\t\t\t=\t\"" << this->_index << "\"" << std::endl;
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
//	std::cout << "\t\t_returnBody\t\t=\t\"" << this->_returnBody << "\"" << std::endl;

	std::cout << "\t\t_rootIsSet\t\t=\t" << this->_rootIsSet << std::endl;
	std::cout << "\t\t_errorPageIsSet\t\t=\t" << this->_errorPageIsSet << std::endl;
	std::cout << "\t\t_autoIndexIsSet\t\t=\t" << this->_autoIndexIsSet << std::endl;
	std::cout << "\t\t_indexIsSet\t\t=\t" << this->_indexIsSet << std::endl;
	std::cout << "\t\t_returnIsSet\t\t=\t" << this->_returnIsSet << std::endl;
	std::cout << "\t\t_limitExceptIsSet\t=\t" << this->_limitExceptIsSet << std::endl;
}

Location *	Location::clone(void) const
{
	Location *ptr;

	ptr = new Location(*this);
	return (ptr);
}
