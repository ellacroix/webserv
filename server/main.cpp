#include "Server.hpp"
#include "signal.h"

#include <list>


int main()
{
	std::list<Server>	Servers;
	
	Server Alfred("Alfred", 8001);
	Server Zac("Zac", 8001);
	Server Joe("Joe", 8002);

	Servers.push_back(Alfred);
	Servers.push_back(Zac);
	Servers.push_back(Joe);

	Alfred.start();
	Zac.start();
	Joe.start();

	while (1)
	{
		for (std::_List_iterator<Server> it_s = Servers.begin(); it_s != Servers.end(); it_s++)
		{
			
		}
	}
	Test.loop();
}