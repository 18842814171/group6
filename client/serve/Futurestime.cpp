#pragma once
#include"Futurestime.h"
#include"mail.h"

using namespace std;

Futuretimesql::Futuretimesql()
{
	//cout << "构造1" << endl;
	con = mysql_init(NULL);
	if (!con)
	{
		cout << "SQL Fail to initial" << endl;
		exit(EXIT_FAILURE);
	}
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	//cout << "000" << endl;
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{
		string err_msg = "MySQL Fail to connect! Error: ";
		err_msg += mysql_error(con);
		mysql_close(con); // 连接失败但 con 非 NULL，先关闭
		throw runtime_error(err_msg);
	}
	cout << "[SQL Connect Success] Futuretimesql is connected to database: " << database_name << endl;
	//cout << "111" << endl;
}

Futuretimesql::~Futuretimesql()
{
	//cout << "析构1" << endl;
	mysql_close(con);
}

void Futuretimesql::Insert(futuretime& t)
{
	lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "INSERT INTO futuretime (clientid,chargenode,date,state) VALUES('%s','%s','%s','Not Trigger')", t.clientid.c_str(), t.chargenode.c_str(),t.date.c_str());
	mysql_query(con, sql);
	cout << "Insert Success"<<endl;
}

void Futuretimesql::Update(futuretime& t)
{
	lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "UPDATE futures SET state='Not Trigger' WHERE clientid='%s' AND chargenode='%s' AND date='%s'",t.clientid.c_str(),t.chargenode.c_str(),t.date.c_str());
	mysql_query(con, sql);
	cout << "Update Success"<<endl;
}

void Futuretimesql::Delete(futuretime& t)
{
	lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "UPDATE futuretime SET state='Delete' WHERE clientid='%s' AND chargenode='%s' AND date='%s'", t.clientid.c_str(), t.chargenode.c_str(), t.date.c_str());
	mysql_query(con, sql);
	cout << "Delete Success" << endl;;
}

void Futuretimesql::Select(string condition,std::vector<futuretime>& msg2)
{
	lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM futuretime WHERE clientid='%s'and state!='Delete'", condition.c_str());
	
	mysql_query(con, sql);
	//cout << "1"  << endl;

	MYSQL_RES* res = mysql_store_result(con);
	//cout << "2" << endl;
	MYSQL_ROW row;
	
		while ((row = mysql_fetch_row(res)))
		{
			//cout << "3" << endl;
			futuretime fut;
			fut.clientid = row[0];
			fut.chargenode = row[1];
			fut.date = row[2];
			fut.state = row[3];
			msg2.push_back(fut);
			

		}
		mysql_free_result(res);

		//cout << "4" << endl;
		if (!msg2.size())
		{
			;//	cout << "Not Found" << endl;

		}
		else
			;//	cout << "Successful Found" << endl;
	
}

vector<futuretime> Futuretimesql::Select1(futuretime& t)
{
	lock_guard<std::mutex> lock(m_mutex);
	vector<futuretime> fulist;
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM futuretime WHERE clientid='%s' AND chargenode='%s' AND date='%s'", t.clientid.c_str(), t.chargenode.c_str(), t.date.c_str());
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(res)))
	{
		futuretime fut;
		fut.clientid = row[0];
		fut.chargenode = row[1];
		fut.date = row[2];
		fut.state = row[3];
		fulist.push_back(fut);

	}
	mysql_free_result(res);
	return fulist;
}

void Futuretimesql::Time_Alarm()
{
	lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM futuretime WHERE state='Not Trigger'");

	//// **修正点 1：检查 mysql_query 的返回值**
	//if (mysql_query(con, sql) != 0) {
	//	// 查询失败，con 可能是有效的，但 SQL 语句有问题或网络中断
	//	std::cerr << "MySQL 查询失败: " << mysql_error(con) << " SQL: " << sql << std::endl;
	//	// 退出函数，避免使用无效的 res
	//	return;
	//}

	mysql_query(con, sql);


	MYSQL_RES* res = mysql_store_result(con);

	// **修正点 2：检查 mysql_store_result 的返回值**
	if (res == NULL && mysql_field_count(con) > 0) {
		// mysql_field_count > 0 表示查询成功但获取结果集失败 (例如，内存不足)
		std::cerr << "MySQL Fail to Store :" << mysql_error(con) << std::endl;
		return;
	}

	// 如果 res 是 NULL，但 mysql_field_count 为 0，可能是非 SELECT 语句 (如 UPDATE)，但此处是 SELECT
	if (res == NULL) {
		std::cerr << "WARNING: MySQL Result is NUL,but no faults in inquary .SQL: " << sql << std::endl;
		return;
	}

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res)))
	{
		string t = GetNowTime();
		if (string(row[2]) <= t)
		{
			//cout << "Alarm!" << endl;
			char sql1[1024];
			sprintf_s(sql1, "UPDATE futuretime SET state='Trigger' WHERE clientid='%s' AND chargenode='%s' AND date='%s'", row[0], row[1], row[2]);
			mysql_query(con, sql1);
			char m[1024];
			sprintf_s(m, "This TimeAlamOrder WARNING:chargenode:%s,date:%s", row[1], row[2]);
			mail(row[0], m);
			
		}
	}
	mysql_free_result(res);


}

