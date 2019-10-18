// Base64Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "MyBase64.h"
#include <iostream>
#include <string.h>

MyBase64 obj;

int main(int argc, char **argv)
{
	obj.generateDecoder();
	obj.showDecoder();
	printf(">\r\n");

	printf(">");

	unsigned char *buf = NULL;
	if (strcmp(argv[1], "-d") == 0)
	{
		buf = obj.base64_decode(reinterpret_cast<unsigned char*>(argv[2]));
		printf("%s\n", buf);
	}
	else
	{
		buf = obj.base64_encode(reinterpret_cast<unsigned char*>(argv[1]));
		printf("%s\n", buf);
	}
	
	free(buf);

	system("pause");

	return 0;
}


/*
使用说明：
////////////////////////////////
加密encode: 
> Base64Test 123456789
  密文 xxxxxxxxxxxxxx
	
////////////////////////////////
解密decode: 
> Base64Test -d xxxxxxxxxxxxxx
  明文 123456789
*/