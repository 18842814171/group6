#pragma once
#include "mysql.h"
#include <string>
#include<iostream>
#include<vector>
#include"Login.h"
#include"Futures.h"
#include"Futurestime.h"
#include"Notice.h"
using namespace std;

class Abi {

	Abi();
	~Abi();

	
public:
  static Abi* GetInstance()
	{
	   static Abi ABis;
		return &ABis;
	}

public:

	bool Register_Judge(string c1,string c2);
	bool Login_Judge(string c1, string c2);
	void Password_Update(string c1, string c2);
	void Account_Delete(string c);
	bool Price_Insert(Future& t);
	void Price_Delete(Future& t);
	void Price_Show(string clientid,std::vector<Future>&);
	string Time_Get();
	bool Time_Insert(futuretime& t);
	void Time_Delete(futuretime& t);
	void Time_Show(string clientid,std::vector<futuretime>&);
	void Notice_Insert(notice& t);
	vector<notice> Notice_Show(string clientid);
	void Price_Alarm(string chargenode, double lastprice);
    static void Time_Alarm();
	
 };

