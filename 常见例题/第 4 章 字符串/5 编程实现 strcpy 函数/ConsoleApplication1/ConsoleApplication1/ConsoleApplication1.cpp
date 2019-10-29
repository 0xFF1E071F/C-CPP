// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

size_t mystrlen(const char* s);

char* mystrcpy(char* dst,
	const char* src);

int main()
{
	char strSrc[] = "Hello World!";
	char strDst[20] = { '\0' };
	printf("strSrc len:%d\n", mystrlen(strSrc));
	mystrcpy(strDst, strSrc);
	strDst[mystrlen(strSrc)] = '\0';
	printf("%s\n", strDst);
	return 0;
}

char* mystrcpy(char* dst, const char* src)
{
	if (dst == NULL || src == NULL) {
		return NULL;
	}
	//保存目标字符串的首地址
	char* pdst = dst;
	while (*src != '\0') {
		*dst = *src;
		dst++;
		src++;
	}
	return pdst;
}

size_t mystrlen(const char* s)
{
	int nlen = 0;
	while (*s != '\0') {
		s++;
		nlen++;
	}
	return nlen;
}