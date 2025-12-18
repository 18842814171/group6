#include "mysql.h"
#include <string>
#include<iostream>
#include<vector>
#pragma once
using namespace std;
typedef struct login {
	string account;
	string password;

}login;




class Loginsql
{

	Loginsql();
	~Loginsql();

public:
   static  Loginsql* GetInstance()
	{
	   static Loginsql Loginsqls;
		return &Loginsqls;
	}
public:
	void Insert(string c1,string c2);
	void Update(string c1,string c2);
	void Delete(string contidion = "");
	bool RegisterSelect(string condition = "");
	bool LoginSelect(string condition1 = "", string condition2 = "");

private:
	MYSQL* con;
	const char* host = "localhost";
	const char* user = "root";
	const char* pw = "CuKjD1pk";//mysql密码
	const char* database_name = "game";//数据库名称
	const int port = 3306;
};

