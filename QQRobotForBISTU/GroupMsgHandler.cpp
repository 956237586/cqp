#include "stdafx.h"
#include "GroupMsgHandler.h"
#include "Util.h"
#include "DBUtil.h"
#include "Robot.h"
#include "PersonInfo.h"
#include "CQCode.h"
#include "GroupMemberInfo.h"
#include <regex>
#include <map>
regex GroupMsgHandler::patternCard1("^(计类|软工|网工|计科)[0-9]{4} .{2,30}", regex_constants::extended);
regex GroupMsgHandler::patternCard2("^[01][0-9][^ ].{1,7} .{2,30}", regex_constants::extended);
regex GroupMsgHandler::patternCard3("^[01][0-9] [^ ].{1,7} .{2,30}", regex_constants::extended);
DBUtil GroupMsgHandler::db;
GroupMsgHandler::GroupMsgHandler() {
}

GroupMsgHandler::~GroupMsgHandler() {
}

int32_t GroupMsgHandler::handle(Message& m) {
	bool reply = false;
	string content(m.content);
	if (m.fromGroup == 647283125 && !content.substr(0, string("回复").size()).compare("回复")) {
		char msg[600] = { 0 };
		char log[600] = { 0 };
		QQ qq = 956237586;
		sscanf(content.substr(4, content.size() - 4).c_str(), "%d %s", &qq, msg);
		Robot::addLog(CQLOG_DEBUG, "人工回复", content.c_str());
		int begin = 0;
		for (int i = 0; i < content.size(); i++) {
			if (content.at(i) == ' ') {
				begin = i;
				break;
			}
		}
		sprintf(log, "qq = %u, content = %s", qq & 0xFFFFFFFF, msg);
		Robot::addLog(CQLOG_DEBUG, "人工回复", log);
		Robot::sendPrivateMsg(qq, content.substr(begin + 1));
	} else {
		bool isFormatCoorect = true;
		char log[100] = { 0 };
		//无视tx的机器人
		if (m.fromQQ == 2854196306) {
			return EVENT_IGNORE;
		}
		bool needCheck = false;
		string sql = "SELECT `count` FROM `db_users`.`group_card_validate` WHERE `qq` = ? AND `group`= ?;";
		sql::PreparedStatement *stmt = db.prepareStatement(sql);
		stmt->setInt(1, m.fromQQ);
		stmt->setInt(2, m.fromGroup);
		stmt->execute();
		sql::ResultSet* rs = stmt->getResultSet();
		if (rs->rowsCount() <= 0) {
			needCheck = true;
			string sql = "INSERT INTO `db_users`.`group_card_validate` VALUES(?,?,?);";
			sql::PreparedStatement *stmt = db.prepareStatement(sql);
			stmt->setInt(1, m.fromQQ);
			stmt->setInt(2, m.fromGroup);
			stmt->setInt(3, 0);
			stmt->execute();
		} else {
			rs->next();
			int count = rs->getInt(1);
			if (count <= 0) {
				needCheck = true;
			}
		}
		if (needCheck) {
			GroupMemberInfo info(Robot::getGroupMemberInfo(m.fromGroup, m.fromQQ, true));
			Robot::addLog(CQLOG_DEBUG, "名片校验", info.groupCard);
			if (m.fromGroup == 639251585) {
				//大群
				isFormatCoorect = regex_match(info.groupCard, patternCard2);
			} else if (m.fromGroup == 615934495) {
				//院群
				isFormatCoorect = regex_match(info.groupCard, patternCard1) || regex_match(info.groupCard, patternCard3);
			}

			if (!isFormatCoorect) {
				string formatErrorMsg = CQCode::at(m.fromQQ) + "名片格式错误！";
				char log[600] = { 0 };
				sprintf(log, "名片格式错误，qq=%u, name = ", m.fromQQ & 0xFFFFFFFF);
				Robot::addLog(CQLOG_DEBUG, "名片校验", string(string(log) + info.groupCard).c_str());
				Robot::sendGroupMsg(m.fromGroup, formatErrorMsg);
			} else {
				string sql = "UPDATE `db_users`.`group_card_validate` SET `count` = 1000 WHERE `qq` = ? AND `group`= ?;";
				sql::PreparedStatement *stmt = db.prepareStatement(sql);
				stmt->setInt(1, m.fromQQ);
				stmt->setInt(2, m.fromGroup);
				stmt->execute();
				sprintf(log, "名片校验通过！qq=%u", m.fromQQ & 0xFFFFFFFF);
				Robot::addLog(CQLOG_DEBUG, "名片校验", log);
			}
		} else {
			string sql = "UPDATE `db_users`.`group_card_validate` SET `count` = `count` - 1 WHERE `qq` = ? AND `group`= ?;";
			sql::PreparedStatement *stmt = db.prepareStatement(sql);
			stmt->setInt(1, m.fromQQ);
			stmt->setInt(2, m.fromGroup);
			stmt->execute();
		}

	}
	if (m.isFromMaster()) {
		//Robot::sendGroupMsg(m.fromGroup, "hi master~");
		//Robot::addLog(CQLOG_INFOSEND, "test", "hi~master~~");
		//string str = CQCode::image("0A535B0E538FFAE8A1AA6AEF7CA61F6C.jpg.cqimg");
		//Robot::sendGroupMsg(m.fromGroup, Util::getWelComeMsg());
		//Robot::addLog(CQLOG_INFOSEND, "test",str);
	}
	test();
	reply = true;
	if (reply) {
		return EVENT_BLOCK;
	} else {
		return EVENT_IGNORE;
	}

}

void GroupMsgHandler::test() {

}