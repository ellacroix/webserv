#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "webserv.hpp"
# include "Location.hpp"

class	Location ;

class	VirtualServer {

	private:
		//	VIRTUAL SERVER PARAMS
		int									_listenPort;
		std::vector<std::string>			_serverName; // IF MANY NAMES
//		std::string							_serverName;
		unsigned int						_clientMaxBodySize;
		std::map<int, std::string>          _errorPage;

		//	UNDERLYING DATA STRUCTURE	=> Map OF Locations
		std::map<std::string, Location*>	_locationMap;

	public:

		VirtualServer(void);
		~VirtualServer(void);
		VirtualServer(VirtualServer const & src);
		VirtualServer &		operator=(VirtualServer const & rhs);

		//	ALREADY SET BOOLEANS
		bool		_serverNameIsSet;
		bool		_listenPortIsSet;
		bool		_clientMaxBodySizeIsSet;
		bool		_errorPageIsSet;
		bool		_locationIsSet;

		//	SETTERS
		void		setListenPort(int n);
		void		setServerName(std::vector<std::string> v);
		void		setClientMaxBodySize(std::string s);
		//	GETTERS
		int								getListenPort(void) const;
		std::map<int, std::string> &    getErrorPage(void);
		std::vector<std::string> &		getServerName(void);
		unsigned int					get_clientMaxBodySize(void);
//		std::vector<std::string> &		getLimitExcept(void);

		//	UTILITIES
		void				reset(void);
		void				display(void) const ;
		VirtualServer *		clone(void) const ;
		void				addLocation(Location * newLocation);
		bool				validate(void) const;
};

#endif
