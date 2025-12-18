// mduserhandler.cpp

#include "MdAPI.h"
#include <iostream>
#include <Windows.h>


using namespace std;

void CMduserHandler::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult) {
        cout << "账户登录成功" << endl;
        cout << "交易日： " << pRspUserLogin->TradingDay << endl;
    }
    else
        cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

void CMduserHandler::connect()
{
    //创建并初始化API
    m_mdApi = CThostFtdcMdApi::CreateFtdcMdApi();
    m_mdApi->RegisterSpi(this);
    char szAddress[] = "tcp://180.168.146.187:10131"; // 行情服务器地址
    m_mdApi->RegisterFront(szAddress);
    m_mdApi->Init();
}

void CMduserHandler::login()
{
    CThostFtdcReqUserLoginField t = { 0 };
    while (m_mdApi->ReqUserLogin(&t, 1) != 0)
        Sleep(1000);
}

void CMduserHandler::subscribe()
{
    int i;
    char** ppInstrument = new char* [100];
    Sleep(1000);
    cout << "开始订阅" << endl;
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
    cout << "订阅成功" << endl;
}

void CMduserHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    int i;
    char* cmdBuf;
    cmdBuf = pDepthMarketData->InstrumentID;
    char marketDataString[256]; // 假设长度为256足够存储所有信息

    sprintf_s(marketDataString, "%s\t%.2f\t%d\t%.2f\t%.2f\t%d\t%d\t%.2f\t%d\t%s",
        pDepthMarketData->InstrumentID,
        pDepthMarketData->LastPrice,
        pDepthMarketData->Volume,
        pDepthMarketData->AskPrice1,
        pDepthMarketData->BidPrice1,
        pDepthMarketData->BidVolume1,
        pDepthMarketData->AskVolume1,
        pDepthMarketData->Turnover,
        pDepthMarketData->OpenInterest,
        pDepthMarketData->UpdateTime
    );
    //进行广播
    for (auto socket : socketVector) {
        socket.sendPackage(marketDataString);
    }
    /*createSocketAndSend("192.168.111.129", 8000, marketDataString);*/
    /*cout << marketDataString << endl;*/
}

void mytask2(SafeQueue<SOCKET>& socketQueue)
{
    
    while (1) {
        std::unique_lock<std::mutex> lock(CMduserHandler::mtx);
        if (!socketQueue.empty()) {
            /*printf("empty\n");*/
            CMduserHandler::cv.wait(lock);
            SOCKET s;
            /*printf("lock\n");
            int i;*/
            socketQueue.pop(s);
            SOCKET client{ s };
            CMduserHandler::socketVector.push_back(client);
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}