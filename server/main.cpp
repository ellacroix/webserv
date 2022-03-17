#include "Server.hpp"
#include "signal.h"


int main()
{
	Server Test;
	
	Test.start();
	Test.loop();
}