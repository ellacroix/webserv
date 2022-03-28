#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "webserv.hpp"
# include "Location.hpp"

class	VirtualServer {

	private:
		//	VIRTUAL SERVER PARAMS
		int									_listenPort;
		//	std::vector<std::string>			_serverName; // IF MANY NAMES
		std::string							_serverName;
		ssize_t								_clientMaxBodySize;


		//	UNDERLYING DATA STRUCTURE => LOCATIONS
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
		bool		_locationIsSet;

		void		setListenPort(int n);
		void		setServerName(std::string s);

		void		reset(void);
		void		display(void) const ;
};

#endif
