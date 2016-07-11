#ifndef QQROBOT_MESSAGE_H
#define QQROBOT_MESSAGE_H
#include "public.h"
class Message {
public:
	Message();
	Message(int32_t subType, int32_t sendTime, QQ fromQQ, const char *content, int32_t font);
	Message(int32_t subType, int32_t sendTime, GroupId fromGroup, QQ fromQQ, const char *fromAnonymous, const char *content, int32_t font);
	~Message();
	int msgType;//0˽��,1Ⱥ��
	//subType �����ͣ�
	//11 / ���Ժ��� 
	//1 / ��������״̬ 
	//2 / ����Ⱥ 
	//3 / ����������
	int32_t subType; 
	int32_t sendTime;
	GroupId fromGroup;
	QQ fromQQ;
	const char *fromAnonymous;
	const char *content;
	int32_t font;
};

#endif // !MESSAGE_H