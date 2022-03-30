#ifndef PORT_HPP
# define PORT_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "VirtualServer.hpp"

# define BUFFER_SIZE 5000

class	Port {

	private:
		// UNDERLYING DATA STRUCTURES	=>	Map OF Clients
		std::map<int, Client*>			Clients;
		//								=>	List OF VirtualServers
		std::list<VirtualServer*>		_VSList;

	public:
		int					port_number;

		//	CONNECTION PARAMS
		int					listen_socket;
		struct sockaddr_in	server_address;
		int 				addr_len;
		int					max_sd;
		int					ret, on;
		bool				kill_port;



		Port();
		Port(int port);
		~Port();

		//	CONNECTION FUNCTIONS
		int	start();							// listen()
		void disconnectClient(int socket);

		//	UTILITIES
		void	addVS(VirtualServer * newVS);
		void	display(void) const;
};

#endif
