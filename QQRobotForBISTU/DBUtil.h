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
	sql::Connection* connect(string url, string username, string password);
	//��ȡ����
	sql::Connection* getConnection();
	//�жϵ�ǰ�Ƿ����ӳɹ�
	bool isConnected();
	//�л����ݿ�
	void use(string dbName);
	//�ر����ݿ�
	void close();
	//ִ�в�ѯsql
	sql::Statement* createStatement();
	sql::PreparedStatement* prepareStatement(string sql);
	bool execute(string sql);
	sql::ResultSet* executeQuery(string sql);
	int executeUpdate(string sql);
	DBUtil();
	~DBUtil();
};

#endif