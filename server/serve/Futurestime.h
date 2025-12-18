#pragma once
#include "mysql.h"
#include <stdexcept>
#include <string>
#include<iostream>
#include<vector>
#include<time.h>
#include <mutex>
//#include"mail.h"
using namespace std;
typedef struct futuretime {//时间预警
	string clientid;//客户端标识
	string chargenode;//交易代号
	string date;
	string state;//状态

}futuretime;




class Futuretimesql
{
public:
	Futuretimesql();
	~Futuretimesql();

public:
	string GetNowTime()
	{
		time_t t1;
		struct tm t2;
		t1 = time(0);
		localtime_s(&t2,&t1);
		char t3[128] = {};
		sprintf_s(t3, "%04d-%02d-%02d %02d:%02d:%02d", t2.tm_year + 1900,
			t2.tm_mon + 1, t2.tm_mday, t2.tm_hour, t2.tm_min, t2.tm_sec);
		string timesnow = static_cast<string>(t3);
		return timesnow;
	}


public:
    static Futuretimesql* GetInstance()
	{
	    static Futuretimesql Futuretimesqls;
		return &Futuretimesqls;
	}
public:
	void Insert(futuretime& t);
	void Update(futuretime& t);
	void Delete(futuretime& t);
	void Select(string condition,std::vector<futuretime>&);
	vector<futuretime> Select1(futuretime& t);
	void Time_Alarm();

private:
	MYSQL* con;
	mutex m_mutex;//互斥锁
	const char* host = "localhost";
	const char* user = "root";
	const char* pw = "CuKjD1pk";//mysql密码
	const char* database_name = "game";//数据库名称
	const int port = 3306;
	
};
