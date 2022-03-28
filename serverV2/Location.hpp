#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class	Location
{
	private:
		std::string							_prefix;
		std::string							_root;
		std::map<int, std::string>			_errorPage;
		bool								_autoIndex;
//		std::vector<std::string>			_index; // IF MANY INDEXES
		std::string							_index; // IF ONE INDEX
//		int									_limitExcept; // IF BITSHIFTING
		std::vector<std::string>			_limitExcept; // STRINGS
		int									_return;
		std::string							_returnURI;
		std::string							_returnBody;

		bool								_rootIsSet;
		bool								_errorPageIsSet;
		bool								_autoIndexIsSet;
		bool								_indexIsSet;
		bool								_returnIsSet;
		bool								_limitExceptIsSet;

	public:

		Location(void);
		~Location(void);

		void		reset(void);

};

#endif
