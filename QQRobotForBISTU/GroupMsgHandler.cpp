#include "stdafx.h"
#include <regex>
#include <driver.h>
#include <statement.h>
#include <prepared_statement.h>
#include <ctime>
#include <cstdlib>

#include "GroupMsgHandler.h"
#include "Util.h"
#include "DBUtil.h"
#include "Robot.h"
#include "PersonInfo.h"
#include "CQCode.h"
#include "signed.h"
#include "GroupMemberInfo.h"
int GroupMsgHandler::flag = 0;
map<GroupId, int> GroupMsgHandler::count;
map<GroupId, int> GroupMsgHandler::nextInt;
regex GroupMsgHandler::pattern("^ *签.*到 *$", regex_constants::extended);
regex GroupMsgHandler::patternCard("^[01][0-9][^ ].{1,3} .{2,10}", regex_constants::extended);
DBUtil GroupMsgHandler::db;

GroupMsgHandler::GroupMsgHandler() {
	//Robot::addLog(CQLOG_DEBUG,"debug","GroupMsgHandler 1");
}

GroupMsgHandler::~GroupMsgHandler() {
	Robot::addLog(CQLOG_DEBUG, "debug", "GroupMsgHandler 2");
}

int32_t GroupMsgHandler::handle(Message& m) {
	bool reply = false;
	//if (m.isFromMaster()) {
		//Robot::sendGroupMsg(m.fromGroup, "hi master~");
		//Robot::addLog(CQLOG_INFOSEND, "test", "hi~master~~");
		//string str = CQCode::image("0A535B0E538FFAE8A1AA6AEF7CA61F6C.jpg.cqimg");
		//Robot::sendGroupMsg(m.fromGroup, Util::getWelComeMsg());
		//Robot::addLog(CQLOG_INFOSEND, "test", str);
	string content(m.content);
	if (!content.compare("我的金币")) {
		//DBUtil db;
		string rep = CQCode::at(m.fromQQ);
		int score;
		bool exist = Util::getScore(db, m.fromQQ, score);
		if (exist) {
			char buffret[50];
			sprintf(buffret, "当前余额：%d", score);
			rep += string(buffret);
		} else {
			rep += "QAQ你从未签到过，查询失败\n请发送“签到”吧~我会记住你的~";
		}
		Robot::sendGroupMsg(m.fromGroup, rep);
	} else if (regex_match(content, pattern)) {
		Signed s;
		s.sign(m);
		reply = true;
	}/*else if (!content.compare("金币排名")){
		GroupMemberInfo info;
		Robot::addLog(CQLOG_DEBUG, "rank", "0");
		info.parse(Robot::getGroupMemberInfo(m.fromGroup, m.fromQQ, true));
		if (m.isFromMaster() || info.powerLevel >= 2) {
			//DBUtil db;
			string sql = "select nickname, score, (select count(distinct score) from db_users.t_users where score > a.score) + 1 rank from db_users.t_users a where (select count(distinct score) from db_users.t_users where score > a.score) + 1  <= 10 order by score desc";
			string rep;
			sql::PreparedStatement *stmt = db.prepareStatement(sql);
			sql::ResultSet* res = stmt->executeQuery();
			Robot::addLog(CQLOG_DEBUG, "rank", "1");
			char buff[1000];
			while (res->next()) {
				sprintf(buff, "第%d名 QQ:", res->getInt(3));
				rep += string(buff);
				rep += res->getString(1);
				sprintf(buff, ",金币:%d", res->getInt(2));
				rep += string(buff);
				rep += "\n";
			}
			//Robot::addLog(CQLOG_DEBUG, "rank", rep.c_str());
			Robot::sendGroupMsg(m.fromGroup, rep);
		}
	}*/ else if (!content.substr(0, string("金币").size()).compare("金币")) {
		if (m.isFromMaster()) {
			char buff[60];
			QQ qq = 956237586;
			int score;
			int addScore = 0;
			Robot::addLog(CQLOG_DEBUG, "add score", content.c_str());
			//加金币[CQ:at,qq=785631998] 1000
			sscanf(content.substr(4, content.size() - 4).c_str(), "[CQ:at,qq=%d] %d", &qq, &addScore);
			sprintf(buff, "qq = %u", qq & 0xFFFFFFFF);
			Robot::addLog(CQLOG_DEBUG, "add score", buff);
			sprintf(buff, "score = %d", addScore);
			Robot::addLog(CQLOG_DEBUG, "add score", buff);
			//DBUtil db;
			if (addScore != 0) {
				if (Util::getScore(db, qq, score)) {
					string rep = CQCode::at(qq);
					string sql = "UPDATE `db_users`.`t_users` SET `score` = score + ? WHERE `qq` = ?;";
					sql::PreparedStatement *stmt = db.prepareStatement(sql);
					stmt->setInt(1, addScore);
					stmt->setInt(2, qq);
					stmt->execute();
					sprintf(buff, "充值成功~~当前余额：%d", score + addScore);
					Robot::sendGroupMsg(m.fromGroup, rep + string(buff));
				} else {
					Robot::sendGroupMsg(m.fromGroup, "QAQ这个人不存在，请让TA发送“签到”，我会记住TA的~");
				}
			}
		}
	} else if (!content.substr(0, string("转账").size()).compare("转账")) {
		char buff[60];
		QQ qq = 956237586;
		int score = 0;
		int score1 = 0;
		int addScore = 0;
		Robot::addLog(CQLOG_DEBUG, "add score", content.c_str());
		//转账[CQ:at,qq=785631998] 1000
		sscanf(content.substr(4, content.size() - 4).c_str(), "[CQ:at,qq=%d] %d", &qq, &addScore);
		sprintf(buff, "qq = %u", qq & 0xFFFFFFFF);
		Robot::addLog(CQLOG_DEBUG, "add score", buff);
		sprintf(buff, "score = %d", addScore);
		Robot::addLog(CQLOG_DEBUG, "add score", buff);
		//DBUtil db;
		if (addScore > 500) {
			if (Util::getScore(db, qq, score) && 
				Util::getScore(db, m.fromQQ, score1)) {
				if (score1 > 0 && addScore <= score1) {
					string rep = "转账成功~\n";
					rep += CQCode::at(qq);
					string sql = "UPDATE `db_users`.`t_users` SET `score` = score + ? WHERE `qq` = ?;";
					sql::PreparedStatement *stmt = db.prepareStatement(sql);
					stmt->setInt(1, addScore);
					stmt->setInt(2, qq);
					stmt->execute();

					sql = "UPDATE `db_users`.`t_users` SET `score` = score - ? WHERE `qq` = ?;";
					delete stmt;
					stmt = db.prepareStatement(sql);
					stmt->setInt(1, addScore);
					stmt->setInt(2, m.fromQQ);
					stmt->execute();
					delete stmt;

					sprintf(buff, "当前余额：%d\n", score + addScore);
					rep += string(buff);
					rep += CQCode::at(m.fromQQ);
					sprintf(buff, "当前余额：%d", score1 - addScore);
					rep += string(buff);
					Robot::sendGroupMsg(m.fromGroup, rep);
				}
			} else {
				Robot::sendGroupMsg(m.fromGroup, "QAQ两人中有一个不存在，请让TA发送“签到”，我会记住TA的~");
			}
		}
	} else {
		if (flag == 0) {
			flag = 1;
			Robot::addLog(CQLOG_DEBUG, "db", "init");
			Signed::db.initConnections();
		}
		if (nextInt[m.fromGroup] == 0) {
			nextInt[m.fromGroup] = 100;
		}
		char buffc[50];
		sprintf(buffc, "count: %d nextint: %d", count[m.fromGroup] + 1, nextInt[m.fromGroup]);
		Robot::addLog(CQLOG_DEBUG, "count", string(buffc));
		if (++count[m.fromGroup] == nextInt[m.fromGroup]) {
			srand(unsigned(time(0)));
			count[m.fromGroup] = 0;
			int rand1 = rand();
			int addScore = rand1 % 50 + 51;
			//int addScore = rand();
			char buff1[30];
			sprintf(buff1, "rand1: %d", rand1);
			Robot::addLog(CQLOG_DEBUG, "rand", string(buff1));
			sprintf(buff1, "addScore: %d", addScore);
			Robot::addLog(CQLOG_DEBUG, "rand", string(buff1));
			string msg = CQCode::at(m.fromQQ);
			int score;
			if (Util::getScore(db, m.fromQQ, score)) {
				//DBUtil db;
				string sql = "UPDATE `db_users`.`t_users` SET `score` = score + ? WHERE `qq` = ?;";
				sql::PreparedStatement *stmt = db.prepareStatement(sql);
				stmt->setInt(1, addScore);
				stmt->setInt(2, m.fromQQ);
				stmt->execute();
				char buff[60];
				sprintf(buff, "你是第%d句发言的\n奖励金币%d~~,计数清零~\n当前余额：%d",
					nextInt[m.fromGroup], addScore, score + addScore);
				msg += string(buff);
			} else {
				msg += "QAQ我不认识你，金币发不出去\n请发送“签到”，我会记住你的~";
			}
			int rand2 = rand();
			int next = rand2 % 30 + 101;
			char buff2[30];
			sprintf(buff1, "rand2: %d", rand2);
			Robot::addLog(CQLOG_DEBUG, "rand", string(buff1));
			sprintf(buff2, "next int: %d", next);
			Robot::addLog(CQLOG_DEBUG, "rand", string(buff2));
			nextInt[m.fromGroup] = next;
			Robot::sendGroupMsg(m.fromGroup, msg);
			reply = true;
		}

	}
	/* else {
		if (!content.substr(0, 1).compare("#")) {
			if (m.isFromMaster()) {
			}
			reply = true;
		}
	}*/
	//}
	//test();
	if (reply) {
		return EVENT_BLOCK;
	} else {
		return EVENT_IGNORE;
	}

}

void GroupMsgHandler::test() {
	
}