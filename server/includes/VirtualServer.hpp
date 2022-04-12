#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "webserv.hpp"
# include "Location.hpp"

class	Location ;

class	VirtualServer {

	private:
		//	VIRTUAL SERVER PARAMS
		int									_listen_port;
		std::vector<std::string>			_server_name; // IF MANY NAMES
		unsigned int						_client_max_body_size;
		std::map<int, std::string>          _error_page;

		//	UNDERLYING DATA STRUCTURE	=> Map OF Locations
		std::map<std::string, Location*>	_location_map;

	public:

		VirtualServer(void);
		~VirtualServer(void);
		VirtualServer(VirtualServer const & src);
		VirtualServer &		operator=(VirtualServer const & rhs);

		//	ALREADY SET BOOLEANS
		bool		_server_name_is_set;
		bool		_listen_port_is_set;
		bool		_client_max_body_size_is_set;
		bool		_error_page_is_set;
		bool		_location_is_set;

		//	SETTERS
		void		setListenPort(int n);
		void		setServerName(std::vector<std::string> v);
		void		setClientMaxBodySize(std::string s);
		//	GETTERS
		int									getListenPort(void) const;
		std::map<int, std::string> &		getErrorPage(void);
		std::vector<std::string> &			getServerName(void);
//		std::vector<std::string> &			getLimitExcept(void);
		std::map<std::string, Location*> &	getLocationMap(void);
		unsigned int						getClientMaxBodySize(void);

		//	UTILITIES
		void				reset(void);
		void				display(void) const ;
		VirtualServer *		clone(void) const ;
		void				addLocation(Location * new_location);
		bool				validate(void) const;
};

#endif
