#pragma once
#include "ThreadPool.h"
#include <time.h> // 
#include <mutex>
#include <condition_variable>
#include "socketTCP.h"
#include <fcntl.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define FLOOD_PORT 6667
#define MAX_CONNECT_CLIENT 5
#define SERVER_PORT 8001
#define CLIENT_PORT_1 5001
#define CLIENT_PORT_2 5002
#define CLIENT_PORT_3 5003
#define CLIENT_PORT_4 5004
#define CLIENT_PORT_5 5005
/*
此类为服务端入口类
ThreadPool为线程池类
使用线程池只需要传入一个函数，这个函数里定义需要执行的任务。函数既可以是server类内部函数，也可以是全局函数，参考myTask与myTask2
*/




class server
{
public:
	server(const server&) = delete;
	server(const server&&) = delete;
	server& operator=(const server&) = delete;
	server& operator=(const server&&) = delete;
	server(int, int); // 构造函数用于初始化,args:port,最大监听数
	void serverWork2Listen();  // 用于循环监听 TODO
	static void poolTask(std::string&, int port,std::string clientid);
	static SafeQueue<std::string> ipQueue;
	static SafeQueue<int> portQueue;
	~server();
private:
	ThreadPool threadPool;	// 线程池
	SOCKET* listenSocket;

};
