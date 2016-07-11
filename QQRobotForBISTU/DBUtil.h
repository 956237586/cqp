#ifndef QQROBOT_DBUTIL_H
#define QQROBOT_DBUTIL_H
#include "public.h"
#include <mysql_connection.h>
#include <driver.h>
#include <statement.h>

#include <resultset.h>
class DBUtil {
private:
	string url;
	string username;
	string password;
	sql::Driver *driver;
	sql::Connection *conn;
	void example();
public:
	//�������ݿ�
	sql::Connection* getConnection(string url, string username, string password);
	//�жϵ�ǰ�Ƿ����ӳɹ�
	bool isConnected();
	//�л����ݿ�
	void use(string dbName);
	//�ر����ݿ�
	void close();
	//ִ�в�ѯsql
	sql::ResultSet* executeQuery(string sql);

	DBUtil();
	~DBUtil();
};

#endif