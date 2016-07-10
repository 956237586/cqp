#include "stdafx.h"
#include "Robot.h"
#include "DBUtil.h"
#include "Util.h"

using namespace std;
Robot::Robot() {
	privateMsgHandler = new PrivateMsgHandler(authCode);
	groupMsgHandler = new GroupMsgHandler(authCode);
}

void Robot::setAuthCode(int ac) {
	this->authCode = ac;
	privateMsgHandler->setAuthCode(ac);
	groupMsgHandler->setAuthCode(ac);
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

int32_t Robot::handleMsg(Message * m) {
	int32_t ret = 0;
	switch (m->msgType) {
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

