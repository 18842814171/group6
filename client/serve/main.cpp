#pragma once
#include "server.h"
#include"Ability.h"
#include"mail.h"
int main()
{
	try {
		server s(8001, 5);		// args:端口号
		s.serverWork2Listen();
	}
	catch (std::string& e) {
		std::cout << e << std::endl;
	}
	
	return 0;
}
