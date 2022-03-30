#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "webserv.hpp"
# include "Location.hpp"

# define K 1000
# define M 1000000

class	VirtualServer {

	private:
		//	VIRTUAL SERVER PARAMS
		int									_listenPort;
		//	std::vector<std::string>		_serverName; // IF MANY NAMES
		std::string							_serverName;
		ssize_t								_clientMaxBodySize;

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
		bool		_locationIsSet;

		//	SETTERS
		void		setListenPort(int n);
		void		setServerName(std::string s);
		void		setClientMaxBodySize(std::string s);
		//	GETTERS
		int			getListenPort(void) const;

		//	UTILITIES
		void				reset(void);
		void				display(void) const ;
		VirtualServer *		clone(void) const ;
		void				addLocation(Location * newLocation);
};

#endif
