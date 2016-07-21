#include "stdafx.h"
#include <mysql_connection.h>
#include <driver.h>
#include <resultset.h>
#include <statement.h>
#include <prepared_statement.h>

#include "signed.h"
#include "public.h"
#include "Robot.h"
#include "DBUtil.h"
#include "Message.h"
#include "CQCode.h"

bool Signed::exist() {
	DBUtil db;
	string sql = "SELECT * FROM db_users.t_users where qq=?";
	sql::PreparedStatement* stmt = db.prepareStatement(sql);
	stmt->setInt(1, qq);
	sql::ResultSet* res = stmt->executeQuery();
	bool ret = res->next();
	if (ret) {
		score = res->getInt(3);
	}
	return ret;
}

int Signed::insertQQ() {
	int ret = -1;
	//���û��
	DBUtil db;
	string sql = "INSERT INTO `db_users`.`t_users` (`qq`,`nickname`,`score`,`enabled`)"
		" VALUES(?,?,?,?);";
	sql::PreparedStatement* stmt = db.prepareStatement(sql);
	stmt->setInt(1, qq);
	char buff[20];
	sprintf(buff, "%d", qq);
	stmt->setString(2, buff);
	stmt->setInt(3, 100);
	stmt->setInt(4, 1);
	return stmt->executeUpdate();
}

int Signed::initSignRecord() {
	int ret = -1;
	DBUtil db;
	//֮ǰûǩ��
	string sql = "INSERT INTO `db_users`.`t_signedrecord`(`qq`,`lastTime`,`maxTimes`) "
		" VALUES(?, curdate(), 1)";
	sql::PreparedStatement* stmt = db.prepareStatement(sql);
	stmt->setInt(1, qq);
	return stmt->executeUpdate();
}

int Signed::updateRecord() {
	DBUtil db;
	//�����
	string sql = "UPDATE `db_users`.`t_signedrecord` SET "
		"`lastTime` = curdate(),"
		"`maxTimes` = ? "
		"WHERE `qq` = ?;";
	sql::PreparedStatement *stmt = db.prepareStatement(sql);
	stmt->setInt(1, maxTimes);
	stmt->setInt(2, qq);
	int ret = -2;
	ret += stmt->executeUpdate();

	sql = "UPDATE `db_users`.`t_users` SET `score` = score + ? WHERE `qq` = ?;";
	delete stmt;
	stmt = db.prepareStatement(sql);
	stmt->setInt(1, maxTimes * 10);
	stmt->setInt(2, qq);
	ret += stmt->executeUpdate();

	return ret;
}

int Signed::getTimes() {
	DBUtil db;
	string sql = "select count(*) from `db_users`.`t_signedrecord` where lastTime = curdate();";
	sql::PreparedStatement *stmt = db.prepareStatement(sql);
	sql::ResultSet* res = stmt->executeQuery();
	res->next();
	return res->getInt(1);
}

bool Signed::isFirstSigned() {
	bool ret;
	DBUtil db;
	string sql = "SELECT * FROM db_users.t_signedrecord where qq=?";
	sql::PreparedStatement* stmt = db.prepareStatement(sql);
	stmt->setInt(1, qq);

	sql::ResultSet* res = stmt->executeQuery();
	res->next();

	lastTime = res->getString("lastTime");
	maxTimes = res->getInt("maxTimes");

	cout << lastTime << endl;
	cout << maxTimes << endl;

	sql = "select date_sub(curdate(),interval 1 day);";
	delete stmt;
	stmt = db.prepareStatement(sql);
	delete res;
	res = stmt->executeQuery();
	res->next();
	if (!lastTime.compare(res->getString(1))) {
		maxTimes += 1;
	} else {
		maxTimes = 1;
	}

	sql = "select curdate();";
	delete stmt;
	stmt = db.prepareStatement(sql);
	delete res;
	res = stmt->executeQuery();
	res->next();
	string now = res->getString(1);
	if (!lastTime.compare(now)) {
		ret = false;
	}else{
		ret = true;
	}
	return ret;
}

void Signed::success() {
	char buff[100];
	int t = maxTimes * 10;
	sprintf(buff, "ǩ���ɹ�~~\n���ǽ����%d��ǩ����~\n������%d�죬�������%d,��������~\n��ǰ���%d��", 
		getTimes(), maxTimes, t, score + t);
	reply += string(buff);
}

int Signed::sign() {
	int ret = 0;
	reply = CQCode::at(m.fromQQ);
	if (!exist()) {
		ret += insertQQ();
		reply += "�״�ʹ�ñ�ϵͳ�����ͽ��100��\n";
		ret += initSignRecord();
		success();
	} else {
		if (isFirstSigned()){
			ret += updateRecord();
			success();
		} else {
			reply += "�ظ�ǩ����������1����";
			Robot::setGroupBan(m.fromGroup, m.fromQQ, 60);
		}
	}
	Robot::sendGroupMsg(m.fromGroup, reply);
	return ret;
}

Signed::Signed(Message & mm):m(mm){
	this->qq = mm.fromQQ;
	maxTimes = 1;
	score = 100;
	sign();
}

Signed::~Signed() {
}
