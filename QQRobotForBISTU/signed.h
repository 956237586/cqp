#ifndef SIGNED_H
#define SIGNED_H
#include "public.h"
#include "Message.h"
#include "DBUtil.h"
class Signed {
private:
	QQ qq;
	string reply;
	string lastTime;
	int	maxTimes;
	int score;
	static DBUtil db;
	bool exist();
	int insertQQ();
	int initSignRecord();
	int updateRecord();
	int getTimes();//��ȡ�������
	bool isFirstSigned();
	void success();
public:
	int sign(Message& m);
	Signed();
	Signed(Message& mm);
	~Signed();
};

#endif // !SIGNED_H

