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
		std::map<int, Client*>			_clients_map;
		//								=>	List OF VirtualServers
		std::list<VirtualServer*>		_VS_list;
		int					port_number;

		//	CONNECTION PARAMS
		int					listen_socket;
		struct sockaddr_in	server_address;
		int 				addr_len;
		int					ret, on;
		bool				opened;

		Port(void);
		Port(int port);
		~Port(void);

		//	CONNECTION FUNCTIONS
		int		start(void);					// listen()

		//	UTILITIES
		void	addVS(VirtualServer * newVS);
		void	display(void) const;
		bool	validate(void) const;
};

#endif
