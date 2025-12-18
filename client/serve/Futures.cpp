#pragma once
#include "Futures.h"
#include"mail.h"

Futuresql::Futuresql()
{
	std::lock_guard<std::mutex> lock(m_mutex);
    con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{

		exit;
	}
	cout << "[SQL Connect Success] Futuresql is connected to database: " << database_name << endl;
}

Futuresql::~Futuresql()
{
	mysql_close(con);
}


void Futuresql::Insert(Future& t)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "INSERT INTO futures (clientid,chargenode,conditions,price,state) VALUES('%s','%s','%s',%lf,'Not Trigger')", t.clientid.c_str(), t.chargenode.c_str(), t.conditions.c_str(),t.price);
	mysql_query(con, sql);
	cout << "Insert Success"<<endl;
}

void Futuresql::Update(Future& t)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "UPDATE futures SET state='Not Trigger' WHERE clientid='%s' AND chargenode='%s' AND conditions='%s' AND price=%lf",t.clientid.c_str(),t.chargenode.c_str(), t.conditions.c_str(),t.price);
	mysql_query(con, sql);
	cout << "Update Success"<<endl;
}

void Futuresql::Delete(Future& t)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	printf_s("UPDATE futures SET state='%s' WHERE clientid='%s' AND chargenode='%s' AND conditions='%s' AND price=%lf", "Delete", t.clientid.c_str(), t.chargenode.c_str(), t.conditions.c_str(), t.price);
	sprintf_s(sql, "UPDATE futures SET state='%s' WHERE clientid='%s' AND chargenode='%s' AND conditions='%s' AND price=%lf","Delete",t.clientid.c_str(), t.chargenode.c_str(), t.conditions.c_str(), t.price);
	mysql_query(con, sql);
	cout << "Delete Success" << endl;
}

void Futuresql::Select(string condition,std::vector<Future>& msg1)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM futures WHERE clientid='%s'and state!='Delete'", condition.c_str());
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	
	 while ((row = mysql_fetch_row(res)))
   	 {
		Future fut;
		fut.clientid = row[0];
		fut.chargenode = row[1];
		fut.conditions = row[2];
		fut.price = atof(row[3]);
		fut.state = row[4];
		msg1.push_back(fut);
		//cout << "刚取出= " << msg1[0].conditions << endl;

	 }
	 mysql_free_result(res);

	 if (!msg1.size())
	 {
		 ;// cout << "Not Found" << endl;

	 }
	 else
		 ;//cout << "Successful Found" << msg1.size() << endl;
}

vector<Future> Futuresql::Select1(Future& t)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	vector<Future> fulist;
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM futures WHERE clientid='%s' AND chargenode='%s' AND conditions='%s' AND price=%lf", t.clientid.c_str(), t.chargenode.c_str(), t.conditions.c_str(), t.price);
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(res)))
	{
		Future fut;
		fut.clientid = row[0];
		fut.chargenode = row[1];
		fut.conditions = row[2];
		fut.price = atof(row[3]);
		fut.state = row[4];
		fulist.push_back(fut);

	}
	mysql_free_result(res);

	return fulist;
}

void Futuresql::Price_Alarm(string chargenode,double lastprice)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM futures WHERE chargenode='%s' AND state='Not Trigger'",chargenode.c_str());
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(res)))
	{
		//cout << row[1] << " " << lastprice<<" "<<row[2]<<" "<<row[3];
		if (string(row[2]) == ">=")
		{
			//cout << "进来了";
			if (lastprice >= atof(row[3]))
			{
				//cout << "进来了2";
				char sql1[1024];
				sprintf_s(sql1, "UPDATE futures SET state='Trigger' WHERE clientid='%s' AND chargenode='%s' AND conditions='%s' AND price=%lf", row[0], row[1], row[2], atof(row[3]));
				mysql_query(con, sql1);
				char m[1024];
				sprintf_s(m, "该价格预警单预警：chargenode:%s,conditions:%s,price:%lf", row[1], row[2], atof(row[3]));
				mail(row[0], m);
			}
			
			
		}
		else
		{
			if (lastprice <= atof(row[3]))
			{
				char sql1[1024];
				sprintf_s(sql1, "UPDATE futures SET state='Trigger' WHERE clientid='%s' AND chargenode='%s' AND conditions='%s' AND price=%lf", row[0], row[1], row[2], atof(row[3]));
				mysql_query(con, sql1);
				char m[1024];
				sprintf_s(m, "该价格预警单预警：chargenode:%s,conditions:%s,price:%lf", row[1], row[2], atof(row[3]));
				mail(row[0], m);
			}
		}
		
	}
	mysql_free_result(res);


}


