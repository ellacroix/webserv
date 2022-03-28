#ifndef PORT_HPP
# define PORT_HPP

# include "webserv.hpp"
# include "Client.hpp"
# include "VirtualServer.hpp"

# define BUFFER_SIZE 5000

class	Port {

private:

public:
	int					port_number;

	//	CONNECTION PARAMS
	int					listen_socket;
	struct sockaddr_in	server_address;
	int 				addr_len;
	int					max_sd;
	int					ret, on;
	bool				kill_port;


	std::map<int, Client*>			Clients;
	std::list<VirtualServer*>		VirtualServers;

	Port();
	Port(int port);
	~Port();

	int	start();							// listen()

	void disconnectClient(int socket);

};

#endif
