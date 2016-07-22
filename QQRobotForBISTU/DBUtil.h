#ifndef QQROBOT_DBUTIL_H
#define QQROBOT_DBUTIL_H
#include "public.h"
#include <mysql_connection.h>
#include <driver.h>
#include <statement.h>
#include <resultset.h>
#include <stdexcept>
#include <exception>
#include <list>
class DBUtil {
private:
	string url;
	string username;
	string password;
	int size;
	int curSize;
	int maxSize;
	list<sql::Connection *> connList;
	sql::Driver *driver;
	void example();
public:
	//��ȡ����
	sql::Connection* getConnection();
	//ִ�в�ѯsql
	sql::Statement* createStatement();
	sql::PreparedStatement* prepareStatement(string sql);
	bool execute(string sql);
	sql::ResultSet* executeQuery(string sql);
	int executeUpdate(string sql);
	DBUtil();
	~DBUtil();


	//��ʼ�����ӳأ����������������һ����������
	void initConnections(int iInitialSize);
	//��������,����һ��Connection
	sql::Connection* createConnection();
	//�������ݿ�����
	void releaseConnection(sql::Connection * conn);
	//����һ������
	void destoryConnection(sql::Connection* conn);
};

#endif