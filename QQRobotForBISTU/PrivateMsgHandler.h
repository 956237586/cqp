#ifndef QQROBOT_PRIVATE_MSG_HANDLER_H
#define QQROBOT_PRIVATE_MSG_HANDLER_H
#include "MsgHandler.h"
#include "DBUtil.h"
class PrivateMsgHandler :
	public MsgHandler {
public:
	PrivateMsgHandler();
	~PrivateMsgHandler();
	int32_t handle(Message& m);
private:
	void test();
};

#endif // !PRIVATE_MSG_HANDLER_H