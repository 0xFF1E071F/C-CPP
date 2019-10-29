// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

char* mystrrev(const char* str);

int main()
{
	const char* str = "123456";
	char* strrev = mystrrev(str);
	printf("%s\n", strrev);
	return 0;
}

char* mystrrev(const char* str)
{
	char* ptmp = new char[strlen(str) + 1];
	strcpy(ptmp, str);
	char* pret = ptmp;
	char* pstart = ptmp;
	char* pend = ptmp + strlen(str) - 1;
	while (pstart < pend) {
		*pend ^= *pstart;
		*pstart ^= *pend;
		*pend ^= *pstart;
		pstart++;
		pend--;
	}
	return pret;
}