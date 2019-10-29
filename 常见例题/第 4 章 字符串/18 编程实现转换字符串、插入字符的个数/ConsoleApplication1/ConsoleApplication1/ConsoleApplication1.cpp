// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>

#define warning (disable:4996)

char* transformation(char* str)
{
	int ncount = 1;
	char* p1 = (char*)str;
	char* p2 = p1 + 1;
	char* buf = (char*)malloc(sizeof(char)*(strlen(str) + 1));//临时存放字符串
	while (*p1) {
		if (*p1 == *p2) {
			ncount++;
			p1++;
			p2++;
		}
		else {
			_itoa(ncount, buf, 10);
			strcat(buf, p2);
			*p2 = 0;
			strcat((char*)str, buf);
			int nbits = strlen(buf);
			p2 += nbits;
			p1 = p2;
			p2 = p1 + 1;
			ncount = 1;
		}
	}
	_itoa(ncount, buf, 10);
	strcat((char*)str, buf);
	delete[] buf;
	buf = NULL;
	return str;
}

int main()
{
	printf("please input a string:");
	char str[255] = "";
	scanf("%s", str);
	printf("before transformation:%s\n", str);
	char* pstr = transformation(str);
	printf("after transformation:%s\n", pstr);
	return 0;
}
