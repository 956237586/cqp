#include "stdafx.h"
#include "Robot.h"
#include "DBUtil.h"
#include "Util.h"

using namespace std;

int Robot::authCode = 0;
Robot::Robot() {
	privateMsgHandler = new PrivateMsgHandler();
	groupMsgHandler = new GroupMsgHandler();
}

void Robot::setAuthCode(int ac) {
	authCode = ac;
}

void Robot::enable() {
	this->enabled = true;
}

void Robot::disable() {
	this->enabled = false;
}

bool Robot::isEnabled() {
	return enabled;
}

QQ Robot::getMasterQQ() {
	Util::getMasterQQ();
	return masterQQ;
}

int32_t Robot::handleMsg(Message & m) {
	int32_t ret = 0;
	switch (m.msgType) {
	case 0: 
		//CQ_addLog(authCode, CQLOG_INFORECV, "��private", msg);
		ret = privateMsgHandler->handle(m); 
		break;//˽��
	case 1: 
		//CQ_addLog(authCode, CQLOG_INFORECV, "��group", m->msg);
		ret = groupMsgHandler->handle(m); 
		break;//Ⱥ��
	default: break;
	}
	return ret;
	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
}

Robot::~Robot() {
	delete privateMsgHandler;
	delete groupMsgHandler;
}

void Robot::sendGroupMsg(QQ groupid, const char *msg) {
	CQ_sendGroupMsg(authCode, groupid, msg);
}

void Robot::sendPrivateMsg(QQ QQID, const char * msg) {
	CQ_sendPrivateMsg(authCode, QQID, msg);
}

void Robot::addLog(int32_t priority, const char * category, const char * content) {
	CQ_addLog(authCode, priority, category, content);
}

void Robot::sendLike(QQ QQID) {
	CQ_sendLike(authCode, QQID);
}

void Robot::setGroupKick(GroupId groupid, QQ QQID, CQBOOL isRejectAddRequestAgain) {
	CQ_setGroupKick(authCode, groupid, QQID, isRejectAddRequestAgain);
}

void Robot::setGroupBan(GroupId groupid, QQ QQID, int64_t duration) {
	CQ_setGroupBan(authCode, groupid, QQID, duration);
}

void Robot::setGroupAdmin(GroupId groupid, QQ QQID, CQBOOL setadmin) {
	CQ_setGroupAdmin(authCode, groupid, QQID, setadmin);
}

void Robot::setGroupWholeBan(GroupId groupid, CQBOOL enableBan) {
	CQ_setGroupWholeBan(authCode, groupid, enableBan);
}

void Robot::setGroupAnonymousBan(GroupId groupid, const char * anomymous, int64_t duration) {
	CQ_setGroupAnonymousBan(authCode, groupid, anomymous, duration);
}

void Robot::setGroupAnonymous(GroupId groupid, CQBOOL enableAnomymous) {
	CQ_setGroupAnonymous(authCode, groupid, enableAnomymous);
}

void Robot::setGroupCard(GroupId groupid, QQ QQID, const char * newCard) {
	CQ_setGroupCard(authCode, groupid, QQID, newCard);
}

void Robot::setGroupLeave(GroupId groupid, CQBOOL isDismiss) {
	CQ_setGroupLeave(authCode, groupid, isDismiss);
}

void Robot::setGroupSpecialTitle(GroupId groupid, QQ QQID, const char * newSpecialtitle, int64_t duration) {
	CQ_setGroupSpecialTitle(authCode, groupid, QQID, newSpecialtitle, duration);
}

void Robot::setDiscussLeave(int64_t discussid) {
	CQ_setDiscussLeave(authCode, discussid);
}

void Robot::setFriendAddRequest(const char * responseFlag, int32_t responseOperation, const char * remark) {
	CQ_setFriendAddRequest(authCode, responseFlag, responseOperation, remark);
}

void Robot::setGroupAddRequest(const char * responseFlag, int32_t requestType, int32_t responseOperation, const char * reason) {
	CQ_setGroupAddRequestV2(authCode, responseFlag, requestType, responseOperation, reason);
}

