#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client {

private:

protected:

public:
	int					stream_socket;
	bool				connected;

	//Request				*request = NULL;
	//Response			*response = NULL;

	Client() {};
	Client(int fd);
	~Client() {};

};

#endif
