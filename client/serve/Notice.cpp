#include"Notice.h"

Note::Note()
{
	con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{

		exit;
	}
}

Note::~Note()
{
	mysql_close(con);
}

void Note::Insert(notice& t)
{
	char sql[1024];
	sprintf_s(sql, "SELECT * FROM notice WHERE clientid='%s' AND chargenode='%s'",t.clientid.c_str(),t.chargenode.c_str());
	mysql_query(con, sql);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	row = mysql_fetch_row(res);
	if (row == nullptr)
	{
		char sql1[1024];
		sprintf_s(sql1, "INSERT INTO notice (clientid,chargenode) VALUES('%s','%s')", t.clientid.c_str(), t.chargenode.c_str());
		mysql_query(con, sql1);
	}
	

}

vector<notice> Note::Show(string condition)
{
	
		vector<notice> fulist;
		char sql[1024];
		sprintf_s(sql, "SELECT * FROM notice WHERE clientid='%s'",condition.c_str());
		mysql_query(con, sql);
		MYSQL_RES* res = mysql_store_result(con);
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(res)))
		{
			notice fut;
			fut.clientid = row[0];
			fut.chargenode = row[1];
			fulist.push_back(fut);

		}
		if (!fulist.size())
		{
			cout << "Not Found" << endl;

		}
		else
			cout << "Successful Found" << endl;
		return fulist;
	
}
