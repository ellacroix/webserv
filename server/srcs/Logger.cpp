#include "Logger.hpp"

Logger::Logger(std::string filename)
{
	filename = "logs/" + filename + ".log";
	this->myfile.open(filename.c_str());

	this->myfile << "Start" << std::endl;
}

void	Logger::log(std::string line)
{
	this->myfile << line << std::endl;
}