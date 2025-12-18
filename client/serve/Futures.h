#pragma once
#include "mysql.h"
#include <string>
#include<iostream>
#include<vector>
#include <mutex>
//#include"mail.h"
using namespace std;
typedef struct Future {
	string clientid;//客户端标识
	string chargenode;//交易代号
	string conditions;//条件，>=或<=
	double price;//阈值
	string state;//触发状态,Not Trigger,Trigger,Delete

}Future;




class Futuresql
{
public:
	Futuresql();
	~Futuresql();

public:
  static  Futuresql* GetInstance()
	{
	   static Futuresql Futuresqls;
		return &Futuresqls;
	}
public:
	void Insert(Future& t);
	void Update(Future& t);
	void Delete(Future& t);
	void Select(string condition,std::vector<Future>&);
	vector<Future> Select1(Future& t);
	void Price_Alarm(string chargenode,double laseprice);

	
private:
	MYSQL* con;
	mutex m_mutex;//互斥锁
	const char* host = "localhost";
	const char* user = "root";
	const char* pw = "CuKjD1pk";//mysql密码
	const char* database_name = "game";//数据库名称
	const int port = 3306;
}; 
