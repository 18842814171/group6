#include "Login.h"
#pragma once
Loginsql::Loginsql()
{
	con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{

		exit;
	}
	cout << "[SQL Connect Success] Loginsql is connected to database: " << database_name << endl;
}

Loginsql::~Loginsql()
{
	mysql_close(con);
}

void Loginsql::Insert(string c1,string c2)
{
	char sql[1024];
	sprintf_s(sql, "INSERT INTO login (account,password) VALUES('%s','%s')", c1.c_str(), c2.c_str());
	mysql_query(con, sql);
	cout << "增添成功!" << endl;
}

void Loginsql::Update(string c1,string c2)
{
	char sql[1024];
	sprintf_s(sql, "UPDATE login SET password='%s' WHERE account='%s'", c2.c_str(), c1.c_str());
	mysql_query(con, sql);
	cout << "更新成功" << endl;
}

void Loginsql::Delete(string condition)
{
	char sql[1024];
	sprintf_s(sql, "DELETE FROM login WHERE account='%s'",condition.c_str());
	mysql_query(con, sql);
	cout << "删除成功"<<endl;
}

bool Loginsql::RegisterSelect(string condition)
{
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM login WHERE account='%s'", condition.c_str());
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	row = mysql_fetch_row(res);
	mysql_free_result(res);

	if (row == nullptr)
		return false;
	else
		return true;
	
}

bool Loginsql::LoginSelect(string condition1, string condition2)
{
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM login WHERE account='%s' AND password='%s'", condition1.c_str(),condition2.c_str());
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	row = mysql_fetch_row(res);
	mysql_free_result(res);

	if (row == nullptr)
		return false;
	else
		return true;
}


