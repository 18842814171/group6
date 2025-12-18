#pragma once
#include <WinSock2.h> //适用平台 Windows
#include <iostream>
#include <string>

using namespace std;
#pragma comment(lib, "ws2_32.lib") /*链接ws2_32.lib动态链接库*/
#pragma warning(disable:4996) 
#pragma once

string base64(string);
int mail(string, string);
