#include "Location.hpp"

// CONSTRUCTORS / DESTRUCTOR

Location::Location(void) :
	_autoIndex(false),
	_returnCode(-1),
//	_client_max_body_size(0),
	_rootIsSet(false),
//	_error_page_is_set(false),
	_autoIndexIsSet(false),
	_indexIsSet(false),
	_returnIsSet(false),
	_limitExceptIsSet(false),
	_uploadFldIsSet(false)//,
//	_client_max_body_size_is_set(false)
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
//	_error_page(src._error_page),
	_autoIndex(src._autoIndex),
	_index(src._index),
	_limitExcept(src._limitExcept),
	_returnCode(src._returnCode),
	_returnUri(src._returnUri),
	_uploadFld(src._uploadFld),
//	_client_max_body_size(src._client_max_body_size),
	_rootIsSet(src._rootIsSet),
//	_error_page_is_set(src._error_page_is_set),
	_autoIndexIsSet(src._autoIndexIsSet),
	_indexIsSet(src._indexIsSet),
	_returnIsSet(src._returnIsSet),
	_limitExceptIsSet(src._limitExceptIsSet),
	_uploadFldIsSet(src._uploadFldIsSet)//,
//	_client_max_body_size_is_set(src._client_max_body_size_is_set)
{
	return ;
}

Location &	Location::operator=(Location const & rhs)
{
	this->_prefix = rhs._prefix;
	this->_root = rhs._root;
//	this->_error_page = rhs._error_page;
	this->_autoIndex = rhs._autoIndex;
	this->_index = rhs._index;
	this->_limitExcept = rhs._limitExcept;
	this->_returnCode = rhs._returnCode;
	this->_returnUri = rhs._returnUri;
	this->_uploadFld = rhs._uploadFld;
//	this->_client_max_body_size = rhs._client_max_body_size;
	this->_rootIsSet = rhs._rootIsSet;
//	this->_error_page_is_set = rhs._error_page_is_set;
	this->_autoIndexIsSet = rhs._autoIndexIsSet;
	this->_indexIsSet = rhs._indexIsSet;
	this->_returnIsSet = rhs._returnIsSet;
	this->_limitExceptIsSet = rhs._limitExceptIsSet;
	this->_uploadFldIsSet = rhs._uploadFldIsSet;
//	this->_client_max_body_size_is_set = rhs._client_max_body_size_is_set;
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

void	Location::setUploadFld(std::string s)
{
	this->_uploadFld = s;
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

	this->_client_max_body_size = std::strtoul(s.c_str(), &endPtr, 10);
	if (*endPtr == 'k')
		this->_client_max_body_size *= K;
	if (*endPtr == 'm')
		this->_client_max_body_size *= M;
}
*/

//	GETTERS
/*
std::map<int, std::string> &	Location::getErrorPage(void)
{
	return (this->_error_page);
}
*/

std::string						Location::getPrefix(void) const
{
	return (this->_prefix);
}

std::string						Location::getRoot(void) const
{
	return (this->_root);
}

int								Location::getReturnCode(void) const
{
	return (this->_returnCode);
}

std::string						Location::getReturnUri(void) const
{
	return (this->_returnUri);
}

std::string						Location::getUploadFld(void) const
{
	return (this->_uploadFld);
}

std::vector<std::string> &		Location::getLimitExcept(void)
{
	return (this->_limitExcept);
}

std::vector<std::string> &		Location::getIndex(void)
{
	return (this->_index);
}

bool							Location::getAutoIndex(void) const
{
	return (this->_autoIndex);
}

//	UTILITIES
void	Location::reset(void)
{
	this->_prefix = "";
	this->_root = "";
//	this->_error_page = std::map<int, std::string>();
	this->_autoIndex = false;
//	this->_index = "";
	this->_index = std::vector<std::string>();
	this->_limitExcept = std::vector<std::string>();
	this->_returnCode = -1;
	this->_returnUri = "";
	this->_uploadFld = "";
//	this->_client_max_body_size = 0;

	this->_rootIsSet = false;
//	this->_error_page_is_set = false;
	this->_autoIndexIsSet = false;
	this->_indexIsSet = false;
	this->_returnIsSet = false;
	this->_limitExceptIsSet = false;
	this->_uploadFldIsSet = false;
//	this->_client_max_body_size_is_set = false;
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
	if (this->_error_page.empty() == true)
		std::cout << "\t\t_error_page[ ]\t\t=\tempty" << std::endl;
	else
	{
		map_it = this->_error_page.begin();
		map_ite = this->_error_page.end();
		while (map_it != map_ite)
		{
			std::cout << "\t\t_error_page[" << (*map_it).first << "]\t\t=\t\""
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
	std::cout << "\t\t_uploadFld\t\t=\t\"" << this->_uploadFld << "\"" << std::endl;
//	std::cout << "\t\t_client_max_body_size\t=\t"
//		<< this->_client_max_body_size << std::endl;


	std::cout << "\t\t_rootIsSet\t\t=\t" << this->_rootIsSet << std::endl;
//	std::cout << "\t\t_error_page_is_set\t\t=\t" << this->_error_page_is_set << std::endl;
	std::cout << "\t\t_autoIndexIsSet\t\t=\t" << this->_autoIndexIsSet << std::endl;
	std::cout << "\t\t_indexIsSet\t\t=\t" << this->_indexIsSet << std::endl;
	std::cout << "\t\t_returnIsSet\t\t=\t" << this->_returnIsSet << std::endl;
	std::cout << "\t\t_limitExceptIsSet\t=\t" << this->_limitExceptIsSet << std::endl;
	std::cout << "\t\t_uploadFldIsSet\t\t=\t" << this->_uploadFldIsSet << std::endl;
//	std::cout << "\t\t_client_max_body_size_is_set\t=\t"
//		<< this->_client_max_body_size_is_set << std::endl;
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
//	this->_error_page_is_set = false;
	this->_autoIndexIsSet = false;
	this->_indexIsSet = false;
	this->_returnIsSet = false;
	this->_limitExceptIsSet = false;
	this->_uploadFldIsSet = false;
//	this->_client_max_body_size_is_set = false;
}
