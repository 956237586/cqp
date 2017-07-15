#include "stdafx.h"
#include "GroupMemberInfo.h"
#include "Util.h"
#include "Robot.h"

GroupMemberInfo::GroupMemberInfo() {
}
GroupMemberInfo::GroupMemberInfo(const string& infoString) {
	parse(infoString);
}

GroupMemberInfo::~GroupMemberInfo() {
}


void GroupMemberInfo::parse(const char* infoString) {
	parse(string(infoString));
}

void GroupMemberInfo::parse(const string& infoString) {
	Robot::addLog(CQLOG_DEBUG, "debug", "parse child");
	decodedInfo = Util::decode(infoString);
	location = 0;
	parseGroupId();
	parseQQ();
	parseNickName();
	parseGroupCard();
	parseSex();
	parseAge();
	parseRegion();
	parseAddTime();
	parseLastMessageTime();
	parseLevel();
	parsePowerLevel();
	parseHasBadHistory();
	parseExclusiveTitle();
	parseExclusiveTitleDeadline();
	parseCardChangeble();
}

void GroupMemberInfo::parseGroupId() {
	groupId = parseLongInt();
}

void GroupMemberInfo::parseGroupCard() {
	groupCard = parseString();
}

void GroupMemberInfo::parseRegion() {
	region = parseString();
}

void GroupMemberInfo::parseAddTime() {
	addTime = parseInt();
}

void GroupMemberInfo::parseLastMessageTime() {
	lastMessageTime = parseInt();
}

void GroupMemberInfo::parseLevel() {
	level = parseString();
}

void GroupMemberInfo::parsePowerLevel() {
	powerLevel = parseInt();
}

void GroupMemberInfo::parseExclusiveTitle() {
	exclusiveTitle = parseString();
}

void GroupMemberInfo::parseExclusiveTitleDeadline() {
	exclusiveTitleDeadline = parseInt();
}

void GroupMemberInfo::parseHasBadHistory() {
	if (parseInt()) {
		hasBadHistory = true;
	} else {
		hasBadHistory = false;
	}
}

void GroupMemberInfo::parseCardChangeble() {
	if (parseInt()) {
		cardChangeble = true;
	} else {
		cardChangeble = false;
	}
}
/*
GroupMemberInfo info;
string infoString(Robot::getGroupMemberInfo(20103153, 294269440, true));
info.parse(infoString);
Robot::sendToMaster(infoString);
char* ret = new char[50];
sprintf(ret, "%d", info.groupCard);
Robot::sendToMaster(info.groupCard);

delete ret;

*/