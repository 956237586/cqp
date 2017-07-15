#include "stdafx.h"
#include "DBUtil.h"
#include "Robot.h"
list<sql::Connection *> DBUtil::connList;
sql::Connection * DBUtil::getConnection() {
	sql::Connection*con;
	Robot::addLog(CQLOG_DEBUG, "db", "get connection");
	//char buff[50];
	//sprintf(buff, "size=%d,cursize=%d",size, curSize);

	while (connList.size() > 0) {
		con = connList.front(); //得到第一个连接
		connList.pop_front();   //移除第一个连接
		Robot::addLog(CQLOG_DEBUG, "db", "get first cached conn");
		if (con != NULL){
			if (con->isValid()) {
				Robot::addLog(CQLOG_DEBUG, "db", "first get");
				return con;
			} else {
				Robot::addLog(CQLOG_DEBUG, "db", "delete first");
				delete con;
			}
		}

	}
	Robot::addLog(CQLOG_DEBUG, "db", "can create new");
	while(true){
		con = this->createConnection();
		if (con != NULL) {
			if (con->isValid()) {
				Robot::addLog(CQLOG_DEBUG, "db", "create success");
				return con;
			} else {
				Robot::addLog(CQLOG_DEBUG, "db", "create failed");
				delete con;
			}
		}
	}

	//if (connList.size() > 0) {   //连接池容器中还有连接
	//	con = connList.front(); //得到第一个连接
	//	connList.pop_front();   //移除第一个连接
	//	Robot::addLog(CQLOG_DEBUG, "db", "get first cached conn");
	//	if (con != NULL && !con->isValid()) {   //如果连接已经被关闭，删除后重新建立一个
	//		Robot::addLog(CQLOG_DEBUG, "db", "conn closed,get new");
	//		delete con;
	//		do {
	//			con = this->createConnection();
	//		} while (con == NULL);
	//		//如果连接为空，则创建连接出错
	//	}
	//	Robot::addLog(CQLOG_DEBUG, "db", "return conn1");
	//} else {
	//	Robot::addLog(CQLOG_DEBUG, "db", "can create new");
	//	do {
	//		con = this->createConnection();
	//	} while (con == NULL);
	//	Robot::addLog(CQLOG_DEBUG, "db", "return conn2");
	//}
}

sql::ResultSet * DBUtil::executeQuery(string sql) {
	sql::Statement* stmt = createStatement();
	return stmt->executeQuery(sql);
}

int DBUtil::executeUpdate(string sql) {
	sql::Statement* stmt = createStatement();
	return stmt->execute(sql);
}

sql::Statement * DBUtil::createStatement() {
	sql::Connection * conn = getConnection();
	sql::Statement* stmt = conn->createStatement();
	releaseConnection(conn);
	return stmt;
}

sql::PreparedStatement * DBUtil::prepareStatement(string sql) {
	sql::Connection * conn = getConnection();
	sql::PreparedStatement* stmt = conn->prepareStatement(sql); 
	releaseConnection(conn);
	return stmt;
}

bool DBUtil::execute(string sql) {
	sql::Statement *stmt = createStatement();
	return stmt->execute(sql);
}


DBUtil::DBUtil() {
	//Robot::addLog(CQLOG_DEBUG, "debug", "DBUtil 1");
	this->maxSize = 20;
	//this->url = "tcp://hostname:3306";
	//this->username = "";
	//this->password = "";
	try {
		this->driver = get_driver_instance();
	} catch (sql::SQLException&e) {
		Robot::addLog(CQLOG_ERROR, "db", "驱动连接出错");
		perror("驱动连接出错;\n");
	} catch (std::runtime_error&e) {
		Robot::addLog(CQLOG_ERROR, "db", "运行出错了");
		perror("运行出错了\n");
	}
}

DBUtil::~DBUtil() {
	Robot::addLog(CQLOG_DEBUG, "debug", "DBUtil 2");
	list<sql::Connection*>::iterator icon;
	for (icon = connList.begin(); icon != connList.end(); ++icon) {
		this->destoryConnection(*icon); //销毁连接池中的连接
	}
	connList.clear(); //清空连接池中的连接
}

void DBUtil::initConnections() {
	int iInitialSize = maxSize / 2;
	sql::Connection* conn;
	for (int i = 0; i < iInitialSize; i++) {
		conn = this->createConnection();
		Robot::addLog(CQLOG_DEBUG, "db", "create connection");
		if (conn) {
			connList.push_back(conn);
		} else {
			perror("创建CONNECTION出错");
			Robot::addLog(CQLOG_ERROR, "db", "初始化创建CONNECTION出错");
		}
	}
}

sql::Connection * DBUtil::createConnection() {
	sql::Connection* conn;
	try {
		conn = driver->connect(this->url, this->username, this->password); //建立连接
		return conn;
	} catch (sql::SQLException&e) {
		perror("创建连接出错");
		Robot::addLog(CQLOG_ERROR, "db", "创建连接出错");
		return NULL;
	} catch (std::runtime_error&e) {
		perror("运行时出错");
		Robot::addLog(CQLOG_ERROR, "db", "运行时出错");
		return NULL;
	}
}

void DBUtil::releaseConnection(sql::Connection * conn) {
	if (conn) {
		Robot::addLog(CQLOG_DEBUG, "db", "释放一个连接");
		connList.push_back(conn);
	}
}

void DBUtil::destoryConnection(sql::Connection * conn) {
	Robot::addLog(CQLOG_DEBUG, "db", "销毁一个连接");
	if (conn) {
		try {
			conn->close();
		} catch (sql::SQLException&e) {
			perror(e.what());
		} catch (std::exception&e) {
			perror(e.what());
		}
		delete conn;
	}
}


void DBUtil::refreshConnections() {
	sql::Connection* conn;
	//声明i为迭代器   
	list<sql::Connection *>::iterator i;
	for (i = connList.begin(); i != connList.end(); ++i) {
		Robot::addLog(CQLOG_DEBUG, "db", "refreshing connections");
		conn = *i;
		if (!conn->isValid()) {
			Robot::addLog(CQLOG_DEBUG, "db", "refreshed connection");
			conn->close();
			delete conn;
			connList.remove(conn);
		} else {
			perror("创建CONNECTION出错");
		}
	}
}

void DBUtil::example() {
	//try {
	//	sql::Statement *stmt;
	//	sql::ResultSet *res;

	//	driver = get_driver_instance();
	//	if (driver != NULL)
	//		//CQ_addLog(authCode, CQLOG_INFORECV, "db", "driver load success");
	//	//连接数据库  
	//	conn = driver->connect("tcp://lz.hylstudio.cn:3306", "", "");
	//	if (conn != NULL) {
	//		//CQ_addLog(authCode, CQLOG_INFORECV, "db", "connect success");
	//	} else {
	//		//CQ_addLog(authCode, CQLOG_INFORECV, "db", "connect failed");
	//	}
	//	//选择要连接的数据库  
	//	conn->setSchema("test");
	//	//新建一个执行语句  
	//	stmt = conn->createStatement();
	//	//执行语句并返回结果集  
	//	res = stmt->executeQuery("SELECT * from testtable");
	//	//遍历结果集  
	//	while (res->next()) {
	//		//这里的ID是表中的字段名  
	//		string t = "";
	//		int id = res->getInt("id");
	//		char buffer[10] = { 0 };
	//		sprintf(buffer, "%d", id);
	//		t += buffer;
	//		t += "   ";
	//		t += res->getString("user");
	//		//CQ_addLog(authCode, CQLOG_INFORECV, "db", t.c_str());
	//		//CQ_sendPrivateMsg(authCode, 956237586, t.c_str());
	//	}
	//	
	//	delete res;
	//	delete stmt;
	//	delete conn;

	//} catch (sql::SQLException &e) {
	//	//有异常的情况下，输出异常  
	//	cout << "# ERR: SQLException in " << __FILE__;
	//	cout << "(" << __FUNCTION__ << ") on line "
	//		<< __LINE__ << endl;
	//	cout << "# ERR: " << e.what();
	//	cout << " (MySQL error code: " << e.getErrorCode();
	//	cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	//}

}