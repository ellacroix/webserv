#ifndef PORT_HPP
# define PORT_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "VirtualServer.hpp"

class	Client ;
class	VirtualServer ;

class	Port {

	private:

	public:
		// UNDERLYING DATA STRUCTURES	=>	Map OF Clients
		std::map<int, Client*>			_clientsMap;
		//								=>	List OF VirtualServers
		std::list<VirtualServer*>		_VSList;
		int					port_number;

		//	CONNECTION PARAMS
		int					listen_socket;
		struct sockaddr_in	server_address;
		int 				addr_len;
		int					max_sd;
		int					ret, on;
		bool				kill_port;
		bool				opened;

		Port(void);
		Port(int port);
		~Port(void);

		//	CONNECTION FUNCTIONS
		int		start(void);					// listen()
		void	disconnectClient(int socket);

		//	UTILITIES
		void	addVS(VirtualServer * newVS);
		void	display(void) const;
		bool	validate(void) const;
};

#endif
