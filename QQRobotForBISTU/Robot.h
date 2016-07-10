#ifndef ROBOT_H
#define ROBOT_H
#include "public.h"
#include "Message.h"
#include "MsgHandler.h"
#include "PrivateMsgHandler.h"
#include "GroupMsgHandler.h"

class Robot {
private:
	int authCode;//��Ȩ��
	bool enabled;//�Ƿ���
	int64_t masterQQ;
	MsgHandler * privateMsgHandler;
	MsgHandler * groupMsgHandler;
	//���Ժ���
	void test();
public:
	//���캯��
	Robot();
	//������Ȩ��
	void setAuthCode(int ac);
	//����
	void enable();
	void disable();
	//״̬�ж�
	bool isEnabled();
	//������Ϣ
	int32_t handleMsg(Message* m);
	//��������
	~Robot();
};
#undef DLLCLASS
#endif

