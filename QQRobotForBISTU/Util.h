#ifndef QQROBOT_UTIL_H
#define QQROBOT_UTIL_H
#include "public.h"
#include "DBUtil.h"

class Util {
private:
	static inline bool iSbase64(unsigned char c);
	static const string base64_chars;
public:
	Util();
	~Util();
	static QQ getMasterQQ();
	static string decode(const string & stringToBeDecode);
	static string encode(unsigned char const* bytesToBeEncode, unsigned int len);
	static string getWelComeMsg(GroupId groupId, QQ qq);
};

#endif // !UTIL_H