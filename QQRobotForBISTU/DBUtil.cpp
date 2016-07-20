#include "stdafx.h"
#include "DBUtil.h"

sql::Connection* DBUtil::getConnection(string url, string username, string password) {
	//try {
		//��ȡ����ʵ��
		driver = get_driver_instance();
		if (driver != NULL) {
			//�������ݿ�  
			conn = driver->connect(url, username, password);
		} else {
			//������ȡʧ��
		}
		if (conn != NULL) {
			//���ӳɹ�
		} else {
			//����ʧ��
		}
		
	//} catch (sql::SQLException &e) {
	//	//���쳣������£�����쳣  
	//	cout << "# ERR: SQLException in " << __FILE__;
	//	cout << "(" << __FUNCTION__ << ") on line "
	//		<< __LINE__ << endl;
	//	cout << "# ERR: " << e.what();
	//	cout << " (MySQL error code: " << e.getErrorCode();
	//	cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	//}
	return this->conn;
}

bool DBUtil::isConnected() {
	return conn != NULL;
}

void DBUtil::use(string dbName) {
	conn->setSchema(dbName);
}

sql::ResultSet * DBUtil::executeQuery(string sql) {
	sql::Statement* stmt = conn->createStatement();
	sql::ResultSet* res = stmt->executeQuery(sql);
	return res;
}

void DBUtil::close() {
	conn->close();
}


DBUtil::DBUtil() {
	//url = "tcp://hostname:3306";
	//username = "username";
	//password = "password";
	getConnection(url, username, password);
}

DBUtil::~DBUtil() {
	delete this->conn;
}

void DBUtil::example() {
	try {
		sql::Statement *stmt;
		sql::ResultSet *res;

		driver = get_driver_instance();
		if (driver != NULL)
			//CQ_addLog(authCode, CQLOG_INFORECV, "db", "driver load success");
		//�������ݿ�  
		conn = driver->connect("tcp://lz.hylstudio.cn:3306", "", "");
		if (conn != NULL) {
			//CQ_addLog(authCode, CQLOG_INFORECV, "db", "connect success");
		} else {
			//CQ_addLog(authCode, CQLOG_INFORECV, "db", "connect failed");
		}
		//ѡ��Ҫ���ӵ����ݿ�  
		conn->setSchema("test");
		//�½�һ��ִ�����  
		stmt = conn->createStatement();
		//ִ����䲢���ؽ����  
		res = stmt->executeQuery("SELECT * from testtable");
		//���������  
		while (res->next()) {
			//�����ID�Ǳ��е��ֶ���  
			string t = "";
			int id = res->getInt("id");
			char buffer[10] = { 0 };
			sprintf(buffer, "%d", id);
			t += buffer;
			t += "   ";
			t += res->getString("user");
			//CQ_addLog(authCode, CQLOG_INFORECV, "db", t.c_str());
			//CQ_sendPrivateMsg(authCode, 956237586, t.c_str());
		}
		
		delete res;
		delete stmt;
		delete conn;

	} catch (sql::SQLException &e) {
		//���쳣������£�����쳣  
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	}

}