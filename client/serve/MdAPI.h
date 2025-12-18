// mduserhandler.h

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include "tradeapi/ThostFtdcMdApi.h"
#include "ThreadPool.h"
#include "socketTCP.h"
#include <map>
#include <array>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "Futures.h"
#include <vector>

#pragma comment(lib, "lib/thostmduserapi_se.lib")
#pragma comment(lib, "lib/thosttraderapi_se.lib")
#pragma comment(lib, "lib/WinDataCollect.lib")


class CMduserHandler : public CThostFtdcMdSpi
{
private:
    CThostFtdcMdApi* m_mdApi;
    char marketDataString[10000];     // 保存信息
    Futuresql f1;
public:
    void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void connect();
    void login();
    void subscribe();
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);
    void floodAPI(std::string&);
    static void mytask2();
    static std::vector<std::string> ipVector;
    static std::mutex mtx;
    static std::condition_variable cv;
};

std::condition_variable CMduserHandler::cv;
std::mutex CMduserHandler::mtx;
std::vector<std::string> CMduserHandler::ipVector;
