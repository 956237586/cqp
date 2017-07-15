/*
CREATE BY LZ
*/


#ifndef QQROBOT_CQCODE_H
#define QQROBOT_CQCODE_H

#include "public.h"
#include <iostream>
class CQCode
{
private:
	string ret;
	string format;
	string typeOfMusic;
	string fileName;
	string url;
	QQ qq;
	int value;
	bool magic;
	string generateForAt();
	string generateForRecord();
	string generateForImage();
	string generateForInt();
	string generateForMusic();
	string generateForRich();
	string trans(string str);
	void stringReplace(string & str, const string & strSrc, const string &strDst);
	CQCode();
	CQCode(int type, int value);
	CQCode(QQ qq);
	CQCode(string fileName);
	CQCode(string fileName, bool magic);
	CQCode(string typeOfMusic, int value);
	CQCode(string url, string fileName);
	~CQCode();
public:
	static const int FACETYPE_COMMON = 1;
	static const int FACETYPE_EMOJI = 2;
	static const int FACETYPE_BFACE = 3;
	static const int FACETYPE_SFACE = 4;
	static const int FACETYPE_RPS = 5;
	static const int FACETYPE_DICE = 6;
	static string at(QQ qq);
	static string face(int type, int value);
	static string image(string fileName);
	static string record(string fileName,bool magic);
	static string shareMusic(string typeOfMusic, int value);
	static string shareUrl(string url, string fileName);
};
#endif
/*
	Example
		string str=CQCode::at(29426440);
		创建一个at294269440的CQCode；
		stirng str=CQCode::face(CQCode::FACETYPE_COMMON,140);
		创建一个发表情140号的CQCode(其中FACETYPE_COMMON有其他类型如emoji);
		string str=CQCode::image("1.jpg");
		创建一个发送图片1.jpg的CQCode(图片放在data\image);
		string str=CQCode::record("1.silk",true);
		创建一个发送语音1.silk的CQCode,后面参数仅仅是为了让外观实现变声效果(并没有变声,文件放在data/record)
		string str=CQCode::shareUrl("https//www.baidu.com","1111");
		分享一个链接分享原因为111;
*/
