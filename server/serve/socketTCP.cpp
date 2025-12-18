#include "socketTCP.h"
#include <vector>


socketTCP::socketTCP(int newport)
{
	SOCKET listenSocket;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		WSACleanup();
		throw std::string("[server.cpp->server]Fail to setup socket");
	}

	int optval = 1;
	// 允许套接字绑定到 TIME_WAIT 状态的地址，防止 Bind failed
	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
		// 虽然失败不致命，但最好记录错误
		std::cerr << "[socketTCP WARNING] Failed to set SO_REUSEADDR." << std::endl;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有网卡
	serverAddr.sin_port = htons(newport); // 1024~65535

	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		closesocket(listenSocket);
		WSACleanup();
		throw std::string("[server.cpp->server] Bind failed,Port:");
	}

	if (listen(listenSocket, 3) == SOCKET_ERROR) {
		closesocket(listenSocket);
		WSACleanup();
		throw std::string("[server.cpp->server] Listen failed" + std::to_string(newport));
	}

	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	// 这里会阻塞，直到客户端连接
	clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

	if (clientSocket == INVALID_SOCKET) {
		closesocket(listenSocket);
		//WSACleanup();
		throw std::string("[socketTCP] Accepted failed");
	}

	closesocket(listenSocket);
}

socketTCP::socketTCP(int newport, std::string& ip)
{
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		//WSACleanup();
		throw std::string("[server->server] Fail to setup client socket");
	}

	sockaddr_in serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(newport);
	inet_pton(AF_INET, ip.c_str(), &(serverInfo.sin_addr));

	if (connect(clientSocket, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR) {
		closesocket(clientSocket);
		//WSACleanup();
		//Sleep(15000);
		throw std::string("[socketTCP] Connected failed" + ip + ":" + std::to_string(newport));
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

	if (byte == 0) {
		throw std::string("[socketTCP->recvPackage] Client disconnected (byte=0).");
	}
	if (byte < 0) {
		throw std::string("[socketTCP->recvPackage] Recieve package failed!");
	}
	std::string str(dataNum, 4);
	int intValue = std::stoi(str);
	byte = recv(clientSocket, data, intValue, 0);
	if (byte == 0) {
		throw std::string("[socketTCP->recvPackage] Client disconnected (byte=0).");
	}
	if (byte < 0) {
		throw std::string("[socketTCP->recvPackage] Recieve data package failed!");
	}
	msg = data;
	// 1. 读取头部 4 字节
	//char dataNum[5] = "";
	//int byte = recv(clientSocket, dataNum, 4, 0);

	//// 【关键修正】 检查 byte == 0 (客户端关闭连接)
	//if (byte == 0) {
	//	throw std::string("[socketTCP->recvPackage] Client disconnected.");
	//}
	//if (byte < 0) {
	//	throw std::string("[socketTCP->recvPackage] Recieve package failed!");
	//}

	//// 2. 读取数据体
	//std::string str(dataNum, 4);
	//int intValue = std::stoi(str);
	//char data[10001] = "";
	//byte = recv(clientSocket, data, intValue, 0);

	//// 【关键修正】 检查 byte == 0 (客户端关闭连接)
	//if (byte == 0) {
	//	throw std::string("[socketTCP->recvPackage] Client disconnected.");
	//}
	//if (byte < 0) {
	//	throw std::string("[socketTCP->recvPackage] Recieve data package failed!");
	//}

	//msg = data;
}

void socketTCP::sendPackage(std::string msg)
{
	int leng = msg.length();
	char str[5] = {0};  // 包括'\0'结束符
	sprintf_s(str, "%04d", leng);
	std::string data = str + msg;
	int byte = send(clientSocket, data.c_str(), data.length(), 0);
	if (byte == SOCKET_ERROR) {
		throw std::string("[socketTCP->sendPackage] Fail to send data package!");
	}
}
