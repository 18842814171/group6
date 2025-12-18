#include "server.h"
#include "MdAPI.h"
#include"Ability.h"

SafeQueue<std::string> server::ipQueue;
SafeQueue<int> server::portQueue;

server::server(int port, int backlog)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw std::string("[server->server] WinSock initialization failed, missing WinSock dll file");//初始化WinSock失败, 缺少WinSock dll文件
	}
	listenSocket = new SOCKET;
	*listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*listenSocket == INVALID_SOCKET) {
		WSACleanup();
		throw std::string("[server->server] Fail to setup socket");
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有网卡
	serverAddr.sin_port = htons(port); // 1024~65535

	if (::bind(*listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		closesocket(*listenSocket);
		WSACleanup();
		throw std::string("[server->server] Bind failed");//绑定失败
	}

	if (listen(*listenSocket, backlog) == SOCKET_ERROR) {
		closesocket(*listenSocket);
		WSACleanup();
		throw std::string("[server->server] Listen failed");//监听失败
	}
	server::portQueue.push(CLIENT_PORT_1);
	server::portQueue.push(CLIENT_PORT_2);
	server::portQueue.push(CLIENT_PORT_3);
	server::portQueue.push(CLIENT_PORT_4);
	server::portQueue.push(CLIENT_PORT_5);
}

void server::serverWork2Listen()
{
	cout << "--- serverWork2Listen() START ---" << endl;
	//cout.flush(); // 强制刷新输出

	CMduserHandler apiFlood;

	cout << "--- CMduserHandler created, starting CTP connect ---" << endl;
	//cout.flush(); // 强制刷新输出

	//SOCKET* s1;
	apiFlood.connect();
	apiFlood.login();
	apiFlood.subscribe();

	cout << "--- CTP Initialization Finished ---" << endl;
	//cout.flush(); // 强制刷新输出

	thread Thread(apiFlood.mytask2);
	thread Thread_Time_Alarm(Abi::GetInstance()->Time_Alarm);

	while (true) {
		struct sockaddr_in addr;
		int sockaddr_in_size = sizeof(SOCKADDR_IN);
		char ip[20] = { 0 };
		SOCKET client = accept(*listenSocket, (struct sockaddr*)&addr, &sockaddr_in_size);
		if (client == INVALID_SOCKET) {
			closesocket(*listenSocket);
			WSACleanup();
			throw std::string("[server->serverWork2Listen] Connected failed");// 连接失败
		}
		else {
			inet_ntop(AF_INET, &(addr.sin_addr), ip, sizeof(ip));
			string ip1(ip);

			// ****** 【修改点】添加连接成功确认 ******
			cout << "[SERVER DEBUG] Client connected from IP: " << ip << std::endl;
			cout.flush();

			socketTCP s{ client };
			//std::cout << "连接成功!" << std::endl;
			std::string username, password;
			s.recvPackage(username);
			s.recvPackage(password);
			string account;
			cout << "[CLIENT LOGIN DEBUG] ccount:" << username << endl;
			cout << "[CLIENT LOGIN DEBUG] password:" << password << endl;
			/* 传入账号密码检查 */
			if (username[0] == '@')
			{
				account = username.substr(1, username.size() - 1);
				bool t = Abi::GetInstance()->Register_Judge(account, password);
				if (t)
					;//注册成功
				else
				{
					s.sendPackage("fail");//已存在
					continue;
				}

			}
			else
			{
				bool t = Abi::GetInstance()->Login_Judge(username, password);
				if (t)
					account = username;//成功匹配
				else
				{
					s.sendPackage("fail");//匹配失败
					continue;
				}
			}

			if (true)
			{
				std::cout << "User ONline:" << ip << std::endl;
				int getPort;
				server::portQueue.pop(getPort);
				cout << getPort << endl;
				s.sendPackage(std::to_string(getPort));
				{
					std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
					std::cout << "push before" << std::endl;
					server::ipQueue.push(ip1);
					std::cout << "push after" << std::endl;
				}
				cout << "port=" << getPort << endl;
				threadPool.submitTask(server::poolTask, ip1, getPort, account);
			}
			else {
				s.sendPackage("fail");
			}
		}
	}
	Thread.join();
	Thread_Time_Alarm.join();
}
void server::poolTask(std::string& ip, int port, string clientid)
{

	socketTCP oneClientListen{ port };
	//oneClientListen.sendPackage("life is short.");
	/* 这里连接数据库，便于及时将新预警单提交服务器 */
	string cnt;
	std::vector<Future> msg1;
	std::vector<futuretime> msg2;


	Abi::GetInstance()->Time_Show(clientid, msg2);
	Abi::GetInstance()->Price_Show(clientid,msg1);

	string Priceshow = "";
	string Timeshow = "";
	
	int a = msg1.size(), b = msg2.size();
	//cout << "a,b=" << a <<"," << b << endl;
	for (int i = 0; i < a; i++)
	{
		char t[1000];
		sprintf_s(t,"%s\t%s\t%lf\t-\t%s\t", msg1[i].chargenode.c_str(), msg1[i].conditions.c_str(), msg1[i].price, msg1[i].state.c_str());
		Priceshow += string(t);
	}
	//cout << Priceshow << endl;
	for (int i = 0; i < b; i++)
	{
		char t[1000];
		sprintf_s(t, "%s\t-\t-\t%s\t%s\t", msg2[i].chargenode.c_str(), msg2[i].date.c_str(), msg2[i].state.c_str());
		Timeshow += string(t);
	}

	oneClientListen.sendPackage(Priceshow);//传送价格预警单
	oneClientListen.sendPackage(Timeshow);//传送时间预警单
	//cout << "Priceshow="<<Priceshow << endl;
	//cout << "Timeshow="<<Timeshow << endl;

	//// 【核心同步点 A】：等待客户端就绪信号
	//string ready_ack;

	//// 客户端在 Widget::forwarn_succ() 中处理完初始数据后，应该发送 "READY_FOR_FLOOD"
	//try {
	//	oneClientListen.recvPackage(ready_ack);
	//}
	//catch (std::string& e) {
	//	// 如果客户端在发送确认前就断开，立即退出
	//		{
	//				std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
	//				std::cout << "push before" << std::endl;
	//				server::ipQueue.push(ip);
	//				std::cout << "push after" << std::endl;
	//		}
	//	return;
	//}

	//// 【核心同步点 B】：确认收到信号后，再将 IP 推入广播队列
	//if (ready_ack == "READY_FOR_FLOOD") {
	//	std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
	//	std::cout << "[POOL DEBUG] Received client ready signal. Pushing IP to queue: " << ip << std::endl;
	//	server::ipQueue.push(ip); // 现在在这里推入 IP
	//}
	//else {
	//	std::cerr << "[POOL ERROR] Expected READY_FOR_FLOOD, received: " << ready_ack << std::endl;
	//}

	while (true) {
		string cnt;
		string pw;
		string chargenode;//交易代号
		string conditions;//条件，>=或<=
		string price;//阈值
		string date;
		//cout << "in while" << endl;
		//string msg;
		try {
			oneClientListen.recvPackage(cnt);		// 随时接收预警单信息
			cout <<"cnt="<< cnt << endl;
			switch (atoi(cnt.c_str()))
			{
			case 0://密码更改
			{   oneClientListen.recvPackage(pw);
			string tmnow = Futuretimesql::GetInstance()->GetNowTime();
			    cout << "Operation Time:" << tmnow<<endl;
			    cout << "User" << ip << " tries to change password:";
				Abi::GetInstance()->Password_Update(clientid, pw);
				cout << endl;
				break;
			}
			case 1://价格预警单增添
			{
				oneClientListen.recvPackage(chargenode);//代码
				oneClientListen.recvPackage(conditions);//>=or <=
				oneClientListen.recvPackage(price);//阈值
				Future t = { clientid,chargenode,conditions,atof(price.c_str()) };
				string tmnow = Futuretimesql::GetInstance()->GetNowTime();
				cout << "Operation Time:" << tmnow<<endl;
				cout << "User" << ip <<" performs price alert addition operation:";
				Abi::GetInstance()->Price_Insert(t);
				cout << endl;

				break;
			}
			case 2://时间预警单增添
			{
				oneClientListen.recvPackage(chargenode);
				oneClientListen.recvPackage(date);//2023-11-02 05:12:58
				futuretime t = { clientid,chargenode,date };
				string tmnow = Futuretimesql::GetInstance()->GetNowTime();
				cout << "Operation Time:" << tmnow<<endl;
				cout << "User" << ip << " performs time alert addition operation:";
				Abi::GetInstance()->Time_Insert(t);
				cout << endl;
				break;

			}
			case 3://价格预警单删除
			{   
				cout << "Enter deletion phase" << endl;
				oneClientListen.recvPackage(chargenode);
				oneClientListen.recvPackage(conditions);
				oneClientListen.recvPackage(price);
				cout <<"Delete"<< chargenode << " " << endl;
				Future t = { clientid,chargenode,conditions,atof(price.c_str()) };
				string tmnow = Futuretimesql::GetInstance()->GetNowTime();
				cout << "Operation Time:" << tmnow << endl;
				cout << "User" << ip << " performs price alert deletion operation:";
				Abi::GetInstance()->Price_Delete(t);
				cout << endl;
				break;
			}
			case 4://时间预警单删除
			{
				oneClientListen.recvPackage(chargenode);
				oneClientListen.recvPackage(date);
				futuretime t = { clientid,chargenode,date };
				string tmnow = Futuretimesql::GetInstance()->GetNowTime();
				cout << "Operation Time:" << tmnow<<endl;
				cout << "User" << ip << " performs price alert deletion operation:";
				Abi::GetInstance()->Time_Delete(t);
				cout << endl;
				break;

			}
			case 9:
			{
				//cout << "HeartBeat From" << ip << endl;
				break;
			}
			default:
				cout << "cnt is not 01234";


			}

		}
		catch (std::string& e) {
			portQueue.push(port);
			//cout << "err" << endl;
			auto it = std::find(CMduserHandler::ipVector.begin(), CMduserHandler::ipVector.end(), ip);
			if (it != CMduserHandler::ipVector.end()) {
				CMduserHandler::ipVector.erase(it);
				std::cout << "IP Removed from vector: " << ip << std::endl;
			}
			std::cerr << "[poolTask] Exception (Client Disconnected): " << e << std::endl;

			std::cout << "User OFFline:" << ip << std::endl;
			break;

		}

	}
}


server::~server()
{
	if (listenSocket != nullptr && *listenSocket != INVALID_SOCKET) {
		closesocket(*listenSocket);
	}
	delete listenSocket;
	WSACleanup();	// 关闭 Winsock 库
}





void CMduserHandler::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		std::cout << "Account Login Success" << std::endl;
		std::cout << "Trade Day: " << pRspUserLogin->TradingDay << std::endl;
	}
	else
		std::cerr << "Return ERROR--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

void CMduserHandler::connect()
{
	//创建并初始化API
	cout << "[MarketData CTP API] Start setup and initial" << endl;
	m_mdApi = CThostFtdcMdApi::CreateFtdcMdApi();
	m_mdApi->RegisterSpi(this);
	char szAddress[] = "tcp://182.254.243.31:40011"; // 行情服务器地址
	cout << "[MarketData CTP API] Register CTP server IP address:" << szAddress << endl;
	m_mdApi->RegisterFront(szAddress);
	cout << "[MarketData CTP API] Start initial API connect" << endl;
	m_mdApi->Init();
	cout << "[MarketData CTP API] Initial finish,Waiting connect" << endl;
}

void CMduserHandler::login()
{
	CThostFtdcReqUserLoginField t = { 0 };
	cout << "[MarketData CIP API] Sending request to CTP server" << endl;
	while (m_mdApi->ReqUserLogin(&t, 1) != 0)
	{
		cout << "[MarketData CTP API] Login request fail,retry 1s later..." << endl;
		Sleep(1000);
	}
	cout << "[MarketData CTP API] Login request send,waiting server respond " << endl;
}

void CMduserHandler::subscribe()
{
	int i;
	char** ppInstrument = new char* [100];
	Sleep(1000);
	std::cout << "\nStart Subscribe" << std::endl;
	char instrumentList[91][20] = {
		 "IF2512","IH2512","IC2512","IM2512","TS2603","TF2603","T2603","TL2603",
		"au2602","ag2602","pt2606","pd2606","cu2601","al2601","zn2601","pb2601",
		"ni2601","sn2601","ao2601","ad2601","bc2512","si2601","ps2601","lc2601",
		"rb2601","i2601","hc2601","ss2601","wr2601","SF603","SM601","jm2601",
		"j2601","ZC512","FG601","sp2601","op2602","lg2601","fb2603","bb2601",
		"sc2601","fu2601","lu2601","bu2601","pg2601","TA601","v2601","v2602F",
		"br2601","ru2601","nr2601","l2601","l2602F","PF602","PR601","eg2601",
		"MA601","PL603","pp2601","pp2602F","bz2603","eb2601","UR601","SA601",
		"PX601","SH601","a2601","b2601","m2601","y2601","RS609","RM601",
		"OI601","p2601","PK601","c2601","cs2601","WH601","PM601","rr2601",
		"RI601","JR601","LR601","CF601","SR601","CY601","jd2601","lh2601",
		"AP601","CJ601","ec2602"
	};// 交易代码
	for (i = 0; i < 91; i++)
		ppInstrument[i] = instrumentList[i];
	while (m_mdApi->SubscribeMarketData(ppInstrument, 91) != 0);
	std::cout << "Subscribe Success" << std::endl;
}

void CMduserHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	char* cmdBuf;
	cmdBuf = pDepthMarketData->InstrumentID;

	sprintf_s(marketDataString, "%s\t%.2f\t%.2f\t%.2f\t%d",
		pDepthMarketData->InstrumentID,   // 合约代码
		pDepthMarketData->LastPrice,	  // 最新价
		pDepthMarketData->HighestPrice,   // 最高价
		pDepthMarketData->LowestPrice,    // 最低价
		pDepthMarketData->OpenInterest   // 持仓量
		//pDepthMarketData->UpdateTime      // 最后修改时间
	);
	//进行广播
	//std::cout << "broadcast number:"<<ipVector.size() << std::endl;
	if (!ipVector.empty()) {
		std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
		for (std::string ip : ipVector) {
			floodAPI(ip);
		}
	}
	f1.Price_Alarm(pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);

	//std::cout << marketDataString << std::endl;
}

void CMduserHandler::floodAPI(std::string& ip)
{
	//std::cout << "[SERVER DEBUG] Attempting to flood " << ip << ":" << FLOOD_PORT << std::endl;
	try {
		socketTCP client(FLOOD_PORT, ip);
		//std::cout << "[SERVER DEBUG] Successfully connected to client: " << ip << std::endl;
		client.sendPackage(marketDataString);
		//std::cout << "Message sent successfully." << std::endl;
	}
	catch (std::string& e) {
		std::cerr << "[SERVER ERROR] Flood failed for " << ip << ". Error: " << e << std::endl;

		//// 【核心清理点】：如果连接失败，移除该 IP
		//{
		//	std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
		//	auto it = std::find(CMduserHandler::ipVector.begin(), CMduserHandler::ipVector.end(), ip);
		//	if (it != CMduserHandler::ipVector.end()) {
		//		CMduserHandler::ipVector.erase(it);
		//		std::cout << "[SERVER CLEANUP] Removed disconnected/failed IP: " << ip << std::endl;
		//	}
		//}
		std::cout << e << std::endl;
		return;
	}
}

void CMduserHandler::mytask2()
{
	while (1) {
		{
			std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
			if (!server::ipQueue.empty()) {
				//CMduserHandler::cv.wait(lock);
				std::string ip;
				server::ipQueue.pop(ip);
				ipVector.push_back(ip);
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

