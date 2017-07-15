#include "stdafx.h"
#include "PrivateMsgHandler.h"
#include "DBUtil.h"
#include "Util.h"
#include "Robot.h"
#include "GroupMemberInfo.h"
//--------------test log ------------------

//--------------test log ------------------
PrivateMsgHandler::PrivateMsgHandler() {
}

PrivateMsgHandler::~PrivateMsgHandler() {

}

int32_t PrivateMsgHandler::handle(Message& m) {
	bool reply = false;
	string msg = string(m.content);
	char* buff = new char[200];
	sprintf(buff, "(À´×ÔQQ:%u)", m.fromQQ & 0xFFFFFFFF);
	msg += string(buff);
	Robot::sendGroupMsg(647283125, msg);
	
	if (m.fromQQ == Util::getMasterQQ()) {
	
	}
	//	Sleep(1000);
	reply = true;
	if (reply){
		return EVENT_BLOCK;
	} else{
		return EVENT_IGNORE;
	}
}
