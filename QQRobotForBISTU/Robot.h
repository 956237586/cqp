#ifndef QQROBOT_ROBOT_H
#define QQROBOT_ROBOT_H
#include "public.h"
#include "Message.h"
#include "MsgHandler.h"
#include "PrivateMsgHandler.h"
#include "GroupMsgHandler.h"

class Robot {
private:
	static int authCode;//��Ȩ��
	bool enabled;//�Ƿ���
	QQ masterQQ;
	MsgHandler * privateMsgHandler;
	MsgHandler * groupMsgHandler;
	//���Ժ���
	void test();
public:
	//���캯��
	Robot();
	//����
	void enable();
	void disable();
	//״̬�ж�
	bool isEnabled();
	QQ getMasterQQ();
	//������Ϣ
	int32_t handleMsg(Message& m);
	//��������
	~Robot();
	//������Ȩ��
	static void setAuthCode(int ac);
	//����Ⱥ��
	static void sendGroupMsg(QQ groupid, const char *msg);
	//����˽��
	static void sendPrivateMsg(QQ QQID, const char *msg);
	//������־��¼
	static void addLog(int32_t priority, const char *category, const char *content);
	//����
	static void sendLike(QQ QQID);
	//��Ⱥ���߳�
	static void setGroupKick(GroupId groupid, QQ QQID, CQBOOL isRejectAddRequestAgain);
	//Ⱥ�ڽ���
	static void setGroupBan(GroupId groupid, QQ QQID, int64_t duration);
	//����Ⱥ��
	static void setGroupAdmin(GroupId groupid, QQ QQID, CQBOOL setAdmin);
	//����ȫԱ����
	static void setGroupWholeBan(GroupId groupid, CQBOOL enableBan);
	//��������
	static void setGroupAnonymousBan(GroupId groupid, const char *anomymous, int64_t duration);
	//�����Ƿ���������
	static void setGroupAnonymous(GroupId groupid, CQBOOL enableAnomymous);
	//����Ⱥ��Ƭ
	static void setGroupCard(GroupId groupid, QQ QQID, const char *newCard);
	//��Ⱥ
	static void setGroupLeave(GroupId groupid, CQBOOL isDismiss);
	//����һ��ͷ��
	static void setGroupSpecialTitle(GroupId groupid, QQ QQID, const char *newSpecialtitle, int64_t duration);
	//�뿪������
	static void setDiscussLeave(int64_t discussid);
	//���ܺ�������
	static void setFriendAddRequest(const char *responseFlag, int32_t responseOperation, const char *remark);
	//���ռ�Ⱥ����
	static void setGroupAddRequest(const char *responseFlag, int32_t requestType, int32_t responseOperation, const char *reason);


};
#endif

/*
CQAPI(const char *) CQ_getGroupMemberInfoV2(int32_t AuthCode, GroupId groupid, QQ QQID, CQBOOL nocache);
CQAPI(const char *) CQ_getStrangerInfo(int32_t AuthCode, QQ QQID, CQBOOL nocache);
CQAPI(const char *) CQ_getCookies(int32_t AuthCode);
static void getCsrfToken(int32_t AuthCode);
CQAPI(int64_t) CQ_getLoginQQ(int32_t AuthCode);
CQAPI(const char *) CQ_getLoginNick(int32_t AuthCode);
CQAPI(const char *) CQ_getAppDirectory(int32_t AuthCode);
static void setFatal(int32_t AuthCode, const char *errorinfo);
*/
