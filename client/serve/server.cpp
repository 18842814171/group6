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
		throw std::string("[server->server]初始化WinSock失败,缺少WinSock dll文件");
	}
	listenSocket = new SOCKET;
	*listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*listenSocket == INVALID_SOCKET) {
		WSACleanup();
		throw std::string("[server->server]Fail to setup socket");
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有网卡
	serverAddr.sin_port = htons(port); // 1024~65535

	if (::bind(*listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		closesocket(*listenSocket);
		WSACleanup();
		throw std::string("[server->server]绑定失败");
	}

	if (listen(*listenSocket, backlog) == SOCKET_ERROR) {
		closesocket(*listenSocket);
		WSACleanup();
		throw std::string("[server->server]监听失败");
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
	cout.flush(); // 强制刷新输出

	CMduserHandler apiFlood;

	cout << "--- CMduserHandler created, starting CTP connect ---" << endl;
	cout.flush(); // 强制刷新输出

	//SOCKET* s1;
	apiFlood.connect();
	apiFlood.login();
	apiFlood.subscribe();

	cout << "--- CTP Initialization Finished ---" << endl;
	cout.flush(); // 强制刷新输出

	thread Thread(apiFlood.mytask2);
	//thread Thread_Time_Alarm(Abi::GetInstance()->Time_Alarm);

	while (true) {
		struct sockaddr_in addr;
		int sockaddr_in_size = sizeof(SOCKADDR_IN);
		char ip[20] = { 0 };
		SOCKET client = accept(*listenSocket, (struct sockaddr*)&addr, &sockaddr_in_size);
		if (client == INVALID_SOCKET) {
			closesocket(*listenSocket);
			WSACleanup();
			throw std::string("[server->serverWork2Listen]连接失败");
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
				//cout << getPort << endl;
				s.sendPackage(std::to_string(getPort));
				{
					std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
					//std::cout << "push before" << std::endl;
					server::ipQueue.push(ip1);
					//std::cout << "push after" << std::endl;
				}
			//	cout << "port=" << getPort << endl;
				threadPool.submitTask(server::poolTask, ip1, getPort, account);
			}
			else {
				s.sendPackage("fail");
			}
		}
	}
	Thread.join();
	//Thread_Time_Alarm.join();
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
			//cout <<"cnt="<< cnt << endl;
			switch (atoi(cnt.c_str()))
			{
			case 0://密码更改
			{   oneClientListen.recvPackage(pw);
			string tmnow = Futuretimesql::GetInstance()->GetNowTime();
			    cout << "操作时间:" << tmnow<<endl;
			    cout << "用户" << ip << "进行密码更改操作:";
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
				cout << "操作时间:" << tmnow<<endl;
				cout << "用户" << ip<<"进行价格预警单增添操作:";
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
				cout << "操作时间:" << tmnow<<endl;
				cout << "用户" << ip << "进行时间预警单增添操作:";
				Abi::GetInstance()->Time_Insert(t);
				cout << endl;
				break;

			}
			case 3://价格预警单删除
			{   
				cout << "进入删除阶段" << endl;
				oneClientListen.recvPackage(chargenode);
				oneClientListen.recvPackage(conditions);
				oneClientListen.recvPackage(price);
				cout <<"Delete"<< chargenode << " " << endl;
				Future t = { clientid,chargenode,conditions,atof(price.c_str()) };
				string tmnow = Futuretimesql::GetInstance()->GetNowTime();
				cout << "操作时间:" << tmnow << endl;
				cout << "用户" << ip << "进行价格预警单删除操作:";
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
				cout << "操作时间:" << tmnow<<endl;
				cout << "用户" << ip << "进行时间预警单删除操作:";
				Abi::GetInstance()->Time_Delete(t);
				cout << endl;
				break;

			}
			default:
				cout << "cnt不是01234";


			}

		}
		catch (std::string& e) {
			portQueue.push(port);
			//cout << "err" << endl;
			auto it = std::find(CMduserHandler::ipVector.begin(), CMduserHandler::ipVector.end(), ip);
			if (it != CMduserHandler::ipVector.end()) {
				CMduserHandler::ipVector.erase(it);
			}
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
	char szAddress[] = "tcp://182.254.243.31:30011"; // 行情服务器地址
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
	char instrumentList[65][20] = {
		 "TA401","rb2401","SA401","m2401","RM401","MA401","FG401","v2401",
		"fu2401","y2401","OI401","p2401","UR401","sp2401","eb2312","ag2402",
		"i2401","c2401","eg2401","SR401","hc2401","ru2401","pp2401","CF401",
		"SF402","l2401","bu2401","sc2312","pg2312","cs2401","ni2312","AP401",
		"SM402","PF402","al2312","zn2312","jm2401","au2402","CJ401","lu2402",
		"b2312","ss2312","a2401","jd2401","nr2401","lh2401","PK401","sn2312",
		"T2312","cu2312","IF2311","TS2312","pb2312","si2401","TF2312","IM2311",
		"IC2311","j2401","IH2311","rr2401","bc2401","CY401","fb2401","wr2401",
		"RS407"
	};// 交易代码
	for (i = 0; i < 65; i++)
		ppInstrument[i] = instrumentList[i];
	while (m_mdApi->SubscribeMarketData(ppInstrument, 65) != 0);
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
		for (std::string ip : ipVector) {
			floodAPI(ip);
		}
	}
	f1.Price_Alarm(pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);

	//std::cout << marketDataString << std::endl;
}

void CMduserHandler::floodAPI(std::string& ip)
{
	try {
		socketTCP client(FLOOD_PORT, ip);
		client.sendPackage(marketDataString);
		//std::cout << "Message sent successfully." << std::endl;
	}
	catch (std::string& e) {
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

