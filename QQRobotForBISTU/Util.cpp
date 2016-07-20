#include "stdafx.h"
#include "Util.h"
#include "CQCode.h"
const string Util::base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

Util::Util() {
}


Util::~Util() {
}

QQ Util::getMasterQQ() {
	//�����ݿ�ȡ����QQ
	return 956237586;
}

inline bool Util::iSbase64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

string Util::decode(const string & encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && iSbase64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	return ret;
}

string Util::encode(unsigned char const *bytesToBeEncode, unsigned int len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (len--) {
		char_array_3[i++] = *(bytesToBeEncode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}

string Util::getWelComeMsg(GroupId groupId,QQ qq) {
	string welCome;
	char* buff = new char[20];
	sprintf(buff,"%d", qq);
	welCome += "[CQ:face,id=137]��ӭ" + CQCode::at(qq) + "(" + string(buff) + ")���뱾Ⱥ��\n"
		"[CQ:face,id=137]������ɾ��̾�ţ�����ʽ(16";
	if (groupId == 513289848) {
		welCome += "רҵ";
	} else {
		welCome += " ����";
	}
	welCome += " ��ʵ����)�޸���Ƭ~~\n"
		"[CQ:face,id=137]����ϸ�Ķ�Ⱥ��֪&Ⱥ�������������QAQ������\n"
		"[CQ:face,id=137]Ⱥ�ļ������Ҳ��һЩ���Ͽ��Բο�~\n"
		"ps���Ͻ���Ϸ�����ˣ�Υ�߽��ԣ���";
	delete buff;
	return welCome;
}