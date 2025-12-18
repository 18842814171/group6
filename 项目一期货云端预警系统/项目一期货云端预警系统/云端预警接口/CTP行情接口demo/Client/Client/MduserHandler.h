#pragma once
#include "tradeapi/ThostFtdcMdApi.h"
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib, "lib/thostmduserapi_se.lib")
#pragma comment(lib, "lib/thosttraderapi_se.lib")
#pragma comment(lib, "lib/WinDataCollect.lib")

class CMduserHandler : public CThostFtdcMdSpi
{
private:
    CThostFtdcMdApi* m_mdApi;
public:
    void connect()
    {
        //创建并初始化API
        m_mdApi = CThostFtdcMdApi::CreateFtdcMdApi();
        m_mdApi->RegisterSpi(this);
        char szAddress[] = "tcp://182.254.243.31:30011"; // 行情服务器地址
        m_mdApi->RegisterFront(szAddress);
        m_mdApi->Init();
    }
    //登陆
    void login()
    {
        CThostFtdcReqUserLoginField t = { 0 };
        while (m_mdApi->ReqUserLogin(&t, 1) != 0)
            Sleep(1000);
    }
    // 订阅行情
    void subscribe()
    {
        char** ppInstrument = new char* [50];
        char szRecordName[] = "IF2512"; // 交易代码
        ppInstrument[0] = szRecordName;
        while (m_mdApi->SubscribeMarketData(ppInstrument, 1) != 0) 
            Sleep(1000);
    }
    //接收行情
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
    {
        double dLastPrice = pDepthMarketData->LastPrice;
        printf("OnRtnDepthMarketData\n");
        printf("%f",dLastPrice);
    }
};