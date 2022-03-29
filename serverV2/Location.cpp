#include "Location.hpp"

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

std::map<int, std::string> & Location::getErrorPage(void)
{
	return (this->_errorPage);
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

	std::cout << "_prefix\t\t\t=\t\"" << this->_prefix << "\"" << std::endl;
	std::cout << "_root\t\t\t=\t\"" << this->_root << "\"" << std::endl;
	if (this->_errorPage.empty() == true)
		std::cout << "_errorPage[ ]\t\t=\tempty" << std::endl;
	else
	{
		map_it = this->_errorPage.begin();
		map_ite = this->_errorPage.end();
		while (map_it != map_ite)
		{
			std::cout << "_errorPage[" << (*map_it).first << "]\t\t=\t\""
				<< (*map_it).second << "\"" << std::endl;
			map_it++;
		}
	}
	std::cout << std::boolalpha;
	std::cout << "_autoIndex\t\t=\t" << this->_autoIndex << std::endl;
	std::cout << "_index\t\t\t=\t\"" << this->_index << "\"" << std::endl;
	if (this->_limitExcept.empty() == true)
		std::cout << "_limitExcept[ ]\t\t=\tempty" << std::endl;
	else
	{
		vct_it = this->_limitExcept.begin();
		vct_ite = this->_limitExcept.end();
		i = 0;
		while (vct_it != vct_ite)
		{
			std::cout << "_limitExcept[" << i << "]\t\t=\t\""
				<< *vct_it << "\"" << std::endl;
			vct_it++;
			i++;
		}
	}
	std::cout << "_return\t\t\t=\t" << this->_returnCode << std::endl;
	std::cout << "_returnUri\t\t=\t\"" << this->_returnUri << "\"" << std::endl;
//	std::cout << "_returnBody\t\t=\t\"" << this->_returnBody << "\"" << std::endl;

	std::cout << "_rootIsSet\t\t=\t" << this->_rootIsSet << std::endl;
	std::cout << "_errorPageIsSet\t\t=\t" << this->_errorPageIsSet << std::endl;
	std::cout << "_autoIndexIsSet\t\t=\t" << this->_autoIndexIsSet << std::endl;
	std::cout << "_indexIsSet\t\t=\t" << this->_indexIsSet << std::endl;
	std::cout << "_returnIsSet\t\t=\t" << this->_returnIsSet << std::endl;
	std::cout << "_limitExceptIsSet\t=\t" << this->_limitExceptIsSet << std::endl;
}

