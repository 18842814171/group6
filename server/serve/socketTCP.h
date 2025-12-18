#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <ws2tcpip.h>


class socketTCP
{
public:
	socketTCP(int newport);       // create TCP to listen
	socketTCP(int newport, std::string& ip);         // create TCP to connect
	socketTCP(SOCKET s) { clientSocket = s; };        // create TCP base on WinSock2
	~socketTCP();
	void recvPackage(std::string&);
	void sendPackage(std::string);
private:
	SOCKET clientSocket;
};
