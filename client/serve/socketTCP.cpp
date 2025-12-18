#include "socketTCP.h"



socketTCP::socketTCP(int newport)
{
	SOCKET listenSocket;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		WSACleanup();
		throw std::string("[server.cpp->server]Fail to setup socket");
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有网卡
	serverAddr.sin_port = htons(newport); // 1024~65535

	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		closesocket(listenSocket);
		WSACleanup();
		throw std::string("[server.cpp->server]绑定失败");
	}

	if (listen(listenSocket, 3) == SOCKET_ERROR) {
		closesocket(listenSocket);
		WSACleanup();
		throw std::string("[server.cpp->server]监听失败");
	}
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		closesocket(listenSocket);
		WSACleanup();
		throw std::string("[socketTCP]连接失败");
	}
}

socketTCP::socketTCP(int newport, std::string& ip)
{
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		WSACleanup();
		throw std::string("[server->server]Fail to setup socket");
	}

	sockaddr_in serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(newport);
	inet_pton(AF_INET, ip.c_str(), &(serverInfo.sin_addr));

	if (connect(clientSocket, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR) {
		closesocket(clientSocket);
		WSACleanup();
		throw std::string("[server->server]绑定失败");
	}
}

socketTCP::~socketTCP()
{
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
	}
}

void socketTCP::recvPackage(std::string& msg)
{
	char data[10001] = "";
	char dataNum[5] = "";
	int byte = recv(clientSocket, dataNum, 4, 0);
	if (byte < 0) {
		throw std::string("[socketTCP->recvPackage]接收包头失败!");
	}
	std::string str(dataNum, 4);
	int intValue = std::stoi(str);
	byte = recv(clientSocket, data, intValue, 0);
	if (byte < 0) {
		throw std::string("[socketTCP->recvPackage]接收数据包失败!");
	}
	msg = data;
}

void socketTCP::sendPackage(std::string msg)
{
	int leng = msg.length();
	char str[5];  // 包括'\0'结束符
	sprintf_s(str, "%04d", leng);
	std::string data = str + msg;
	int byte = send(clientSocket, data.c_str(), data.length(), 0);
	if (byte == SOCKET_ERROR) {
		throw std::string("[socketTCP->sendPackage]Fail to send data package!");
	}
}
