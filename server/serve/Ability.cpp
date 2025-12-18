#include"Ability.h"


Abi::Abi()
{
	//cout << "构造" << endl;
}

Abi::~Abi()
{
	//cout << "析构" << endl;
}

bool Abi::Register_Judge(string c1,string c2)//注册判定
{
	bool s = Loginsql::GetInstance()->RegisterSelect(c1);
	if (s)
		return false;//已存在
	else
	{
		Loginsql::GetInstance()->Insert(c1, c2);
		return true;//注册成功
	}
}

bool Abi::Login_Judge(string c1, string c2)//登录判定
{
	bool s = Loginsql::GetInstance()->LoginSelect(c1,c2);
	if (s)
		return true;//成功匹配
	else
	{
		
		return false;//匹配失败
	}
}

void Abi::Password_Update(string c1, string c2)//密码修改
{  
	Loginsql::GetInstance()->Update(c1, c2);
}

void Abi::Account_Delete(string c)//账户注销
{
	Loginsql::GetInstance()->Delete(c);
}

bool Abi::Price_Insert(Future& t)//价格预警单增添
{
	vector<Future>s;
	if (Futuresql::GetInstance() == NULL ) {
		// 记录错误或重新初始化连接
		std::cerr << "CRITICAL: Futuresql pointer or connection handle is NULL before Select1 call!" << std::endl;
		// ... 可能需要重新建立连接或直接返回错误 ...
		return false;
	}
	s=Futuresql::GetInstance()->Select1(t);
	int m = s.size();
	if (m == 0)
	{
		Futuresql::GetInstance()->Insert(t); return true;
	}//增添成功
	else
	{
		if (s[0].state == "Delete"||s[0].state=="Trigger")
		{
			Futuresql::GetInstance()->Update(t); return true;
		}
		else
			return false;//已存在
	}
	
	
}

void Abi::Price_Delete(Future& t)//价格预警单删除
{
	Futuresql::GetInstance()->Delete(t);
}

void Abi::Price_Show(string clientid,std::vector<Future>& msg1)//价格预警单展示
{
	Futuresql::GetInstance()->Select(clientid,msg1);
	/*Futuresql s;
	s.Select(clientid, msg1);*/
	
}

string Abi::Time_Get()//获取当前本地时间
{
	string t;
	t = Futuretimesql::GetInstance()->GetNowTime();
	return t;
}

bool Abi::Time_Insert(futuretime& t)//时间预警单增添
{
	vector<futuretime>s;
	s = Futuretimesql::GetInstance()->Select1(t);
	int m = s.size();
	if (m == 0)
	{
		Futuretimesql::GetInstance()->Insert(t);  return true;
	}//增添成功
	else
	{
		if (s[0].state == "Delete" || s[0].state == "Trigger")
		{
			Futuretimesql::GetInstance()->Update(t); return true;
		}
		else
			return false;//已存在
	}
}

void Abi::Time_Delete(futuretime& t)//时间预警单删除
{
	Futuretimesql::GetInstance()->Delete(t);
}

void Abi::Time_Show(string clientid,std::vector<futuretime>& msg2)//时间预警单展示
{
	Futuretimesql::GetInstance()->Select(clientid,msg2);
}

void Abi::Notice_Insert(notice& t)//信息增添
{
	Note::GetInstance()->Insert(t);
}

vector<notice> Abi::Notice_Show(string clientid)//信息展示
{
	vector<notice>t = Note::GetInstance()->Show(clientid);
	return t;
}

void Abi::Price_Alarm(string chargenode, double lastprice)//价格预警
{
	Futuresql f1;
		f1.Price_Alarm(chargenode, lastprice);
}

void Abi::Time_Alarm()
{
	Futuretimesql f1;
	while (1) {
		//cout << "调用" << endl;
		f1.Time_Alarm();
}
}

