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
		int									_returnCode;
		std::string							_returnUri;
//		std::string							_returnBody;

	public:

		Location(void);
		~Location(void);

		bool								_rootIsSet;
		bool								_errorPageIsSet;
		bool								_autoIndexIsSet;
		bool								_indexIsSet;
		bool								_returnIsSet;
		bool								_limitExceptIsSet;

		void		setPrefix(std::string s);
		void		setRoot(std::string s);
		void		setAutoindex(bool b);
//		void		setIndex(std::vector<std::string> v);
		void		setIndex(std::string s);
		void		setReturnCode(int n);
		void		setReturnUri(std::string s);
		void		setLimitExcept(std::vector<std::string> v);

		std::map<int, std::string> & 	getErrorPage(void);

		void		reset(void);
		void		display(void) const;


};

#endif
