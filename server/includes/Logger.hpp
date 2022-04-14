#ifndef LOGGER_HPP
# define LOGGER_HPP

# include "webserv.hpp"

class	Logger {
	public:
		Logger(){};
		Logger(std::string filename);
		~Logger(void) {};

		std::ofstream 	myfile;
		struct timeval	timestamp;

		void	log(std::string line);

};

#endif
