#pragma once
#include "mysql.h"
#include <string>
#include<iostream>
#include<vector>
using namespace std;
typedef struct notice {
	string clientid;//客户端标识
	string chargenode;//交易代号
}notice;

class Note {

public:
	static Note* GetInstance()
	{
		static Note Notes;
		return &Notes;
	}
public:
	Note();
	~Note();
	void Insert(notice& t);
	vector<notice> Show(string condition);


private:
	MYSQL* con;
	const char* host = "localhost";
	const char* user = "root";
	const char* pw = "CuKjD1pk";//mysql密码
	const char* database_name = "game";//数据库名称
	const int port = 3306;

};