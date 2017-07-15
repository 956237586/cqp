#ifndef QQROBOT_GROUP_MSG_HANDLER_H
#define QQROBOT_GROUP_MSG_HANDLER_H
#include "MsgHandler.h"
#include "DBUtil.h"
#include <regex>
#include <map>
class GroupMsgHandler :
	public MsgHandler {
private:
	void test();
	static DBUtil db;
public:
	static regex patternCard1;
	static regex patternCard2;
	static regex patternCard3;
	GroupMsgHandler();
	~GroupMsgHandler();
	int32_t handle(Message& m);
};

#endif // !GROUP_MSG_HANDLER_H
