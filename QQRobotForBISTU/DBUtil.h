#ifndef QQROBOT_DBUTIL_H
#define QQROBOT_DBUTIL_H
#include "public.h"
#include <mysql_connection.h>
#include <driver.h>
#include <statement.h>
#include <prepared_statement.h>
#include <resultset.h>
#include <stdexcept>
#include <exception>
#include <list>
class DBUtil {
private:
	int flag;
	string url;
	string username;
	string password;
	int maxSize;
	static list<sql::Connection *> connList;
	sql::Driver *driver;
	void example();
public:
	//获取链接
	sql::Connection* getConnection();
	//执行查询sql
	sql::Statement* createStatement();
	sql::PreparedStatement* prepareStatement(string sql);
	bool execute(string sql);
	sql::ResultSet* executeQuery(string sql);
	int executeUpdate(string sql);
	DBUtil();
	~DBUtil();


	//初始化连接池，创建最大连接数的一半连接数量
	void initConnections();
	//创建连接,返回一个Connection
	sql::Connection* createConnection();
	//回收数据库连接
	void releaseConnection(sql::Connection * conn);
	//销毁一个连接
	void destoryConnection(sql::Connection* conn);
	void refreshConnections();
};

#endif