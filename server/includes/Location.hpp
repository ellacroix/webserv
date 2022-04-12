#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class	Location
{
	private:
		std::string							_prefix;
		std::string							_root;
//		std::map<int, std::string>			_error_page;
		bool								_autoIndex;
		std::vector<std::string>			_index; // IF MANY INDEXES
//		std::string							_index; // IF ONE INDEX
//		int									_limitExcept; // IF BITSHIFTING
		std::vector<std::string>			_limitExcept; // STRINGS
		int									_returnCode;
		std::string							_returnUri;
//		ssize_t								_client_max_body_size;
//		std::string							_returnBody;

	public:

		// CONSTRUCTORS / DESTRUCTOR
		Location(void);
		~Location(void);
		Location(Location const & src);
		Location &	operator=(Location const & rhs);

		//	SET BOOLEANS
		bool								_rootIsSet;
//		bool								_error_page_is_set;
		bool								_autoIndexIsSet;
		bool								_indexIsSet;
		bool								_returnIsSet;
		bool								_limitExceptIsSet;
//		bool								_client_max_body_size_is_set;

		//	SETTERS
		void		setPrefix(std::string s);
		void		setRoot(std::string s);
		void		setAutoindex(bool b);
		void		setIndex(std::vector<std::string> v);
//		void		setIndex(std::string s);
		void		setReturnCode(int n);
		void		setReturnUri(std::string s);
		void		setLimitExcept(std::vector<std::string> v);
		void		setClientMaxBodySize(std::string s);

		//	GETTERS
//		std::map<int, std::string> & 	getErrorPage(void);
		std::string						getPrefix(void) const;
		std::string						getRoot(void) const;
		int								getReturnCode(void) const;
		std::string						getReturnUri(void) const;
		std::vector<std::string> &		getLimitExcept(void);
		std::vector<std::string> &		getIndex(void);
		bool							getAutoIndex(void) const;

		//	UTILITIES
		void		reset(void);
		void		display(void) const;
		Location *	clone(void) const;
		bool		validate(void) const;
		void		resetIsDefBooleans(void);

};

#endif
