#include "Location.hpp"

Location::Location(void) :
	_autoIndex(false),
	_return(-1),
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

void	Location::reset(void)
{
	this->_prefix = "";
	this->_root = "";
	this->_errorPage = std::map<int, std::string>();
	this->_autoIndex = false;
	this->_index = "";
	this->_limitExcept = std::vector<std::string>();
	this->_return = -1;
	this->_returnURI = "";
	this->_returnBody = "";

	this->_rootIsSet = false;
	this->_errorPageIsSet = false;
	this->_autoIndexIsSet = false;
	this->_indexIsSet = false;
	this->_returnIsSet = false;
	this->_limitExceptIsSet = false;
}
