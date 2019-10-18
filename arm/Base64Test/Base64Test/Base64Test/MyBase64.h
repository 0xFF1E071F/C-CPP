#pragma once
class MyBase64
{
public:
	MyBase64();
	~MyBase64();
	//加密
	unsigned char *base64_encode(unsigned char *str);
	//解密
	unsigned char *base64_decode(unsigned char *code);
	//生成解码表
	void generateDecoder();
	//显示解码表
	void showDecoder();

private:
	int mDecoderTable[130];
};

