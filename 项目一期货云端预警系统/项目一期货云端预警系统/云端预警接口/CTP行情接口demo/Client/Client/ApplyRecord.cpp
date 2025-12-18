// ApplyRecord.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "MduserHandler.h"
int main()
{
    CMduserHandler* mduser = new CMduserHandler;
    mduser->connect();
    mduser->login();
    mduser->subscribe();
    Sleep(INFINITE);
}
