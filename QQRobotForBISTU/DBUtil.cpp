#include "stdafx.h"
#include "DBUtil.h"
#include "Robot.h"
list<sql::Connection *> DBUtil::connList;
sql::Connection * DBUtil::getConnection() {
	sql::Connection*con;
	Robot::addLog(CQLOG_DEBUG, "db", "get connection");
	char buff[50];
	//sprintf(buff, "size=%d,cursize=%d",size, curSize);
	if (connList.size() > 0) {   //���ӳ������л�������
		con = connList.front(); //�õ���һ������
		connList.pop_front();   //�Ƴ���һ������
		Robot::addLog(CQLOG_DEBUG, "db", "get first cached conn");
		if (!con->isValid()) {   //��������Ѿ����رգ�ɾ�������½���һ��
			Robot::addLog(CQLOG_DEBUG, "db", "conn closed,get new");
			if (!con->reconnect()) {
				con->close();
				delete con;
				con = this->createConnection();
			}
			refreshConnections();
		}
		//�������Ϊ�գ��򴴽����ӳ���
		while (con == NULL) {
			con = createConnection();
		}
		Robot::addLog(CQLOG_DEBUG, "db", "return conn1");
		return con;
	} else {
		Robot::addLog(CQLOG_DEBUG, "db", "can create new");
		do {
			con = this->createConnection();
		} while (con == NULL);
		Robot::addLog(CQLOG_DEBUG, "db", "return conn2");
		return con;
	}
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
	return stmt->execute(sql);;
}


DBUtil::DBUtil() {
	this->maxSize = 20;
	//this->url = "tcp://hostname:3306";
	//this->username = "";
	//this->password = "";
	try {
		this->driver = get_driver_instance();
	} catch (sql::SQLException&e) {
		Robot::addLog(CQLOG_ERROR, "db", "�������ӳ���");
		perror("�������ӳ���;\n");
	} catch (std::runtime_error&e) {
		Robot::addLog(CQLOG_ERROR, "db", "���г�����");
		perror("���г�����\n");
	}
	//initConnections();
}

DBUtil::~DBUtil() {
	list<sql::Connection*>::iterator icon;
	for (icon = connList.begin(); icon != connList.end(); ++icon) {
		this->destoryConnection(*icon); //�������ӳ��е�����
	}
	connList.clear(); //������ӳ��е�����
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
			perror("����CONNECTION����");
			Robot::addLog(CQLOG_ERROR, "db", "��ʼ������CONNECTION����");
		}
	}
}

sql::Connection * DBUtil::createConnection() {
	sql::Connection* conn;
	try {
		conn = driver->connect(this->url, this->username, this->password); //��������
		return conn;
	} catch (sql::SQLException&e) {
		perror("�������ӳ���");
		Robot::addLog(CQLOG_ERROR, "db", "�������ӳ���");
		return NULL;
	} catch (std::runtime_error&e) {
		perror("����ʱ����");
		Robot::addLog(CQLOG_ERROR, "db", "����ʱ����");
		return NULL;
	}
}

void DBUtil::releaseConnection(sql::Connection * conn) {
	if (conn) {
		Robot::addLog(CQLOG_DEBUG, "db", "�ͷ�һ������");
		connList.push_back(conn);
	}
}

void DBUtil::destoryConnection(sql::Connection * conn) {
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
	//����iΪ������   
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
			perror("����CONNECTION����");
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
	//	//�������ݿ�  
	//	conn = driver->connect("tcp://lz.hylstudio.cn:3306", "", "");
	//	if (conn != NULL) {
	//		//CQ_addLog(authCode, CQLOG_INFORECV, "db", "connect success");
	//	} else {
	//		//CQ_addLog(authCode, CQLOG_INFORECV, "db", "connect failed");
	//	}
	//	//ѡ��Ҫ���ӵ����ݿ�  
	//	conn->setSchema("test");
	//	//�½�һ��ִ�����  
	//	stmt = conn->createStatement();
	//	//ִ����䲢���ؽ����  
	//	res = stmt->executeQuery("SELECT * from testtable");
	//	//���������  
	//	while (res->next()) {
	//		//�����ID�Ǳ��е��ֶ���  
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
	//	//���쳣������£�����쳣  
	//	cout << "# ERR: SQLException in " << __FILE__;
	//	cout << "(" << __FUNCTION__ << ") on line "
	//		<< __LINE__ << endl;
	//	cout << "# ERR: " << e.what();
	//	cout << " (MySQL error code: " << e.getErrorCode();
	//	cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	//}

}