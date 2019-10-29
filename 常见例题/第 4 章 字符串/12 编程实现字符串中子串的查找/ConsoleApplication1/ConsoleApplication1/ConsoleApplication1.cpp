// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <assert.h>

char* mystrstr(char* str,
	char* substr);

int main()
{
	char str[] = "hello world!";
	char substr[] = "ll";

	printf("%s\n", mystrstr(str, substr));
	return 0;
}

char* mystrstr(char* str,
	char* substr)
{
	assert(str != NULL && substr != NULL);
	char* pstr = NULL;
	char* psubstr = NULL;

	//遍历str字符串
	while (*str != '\0') {
		pstr = str;
		psubstr = substr;
		//遍历substr字符串
		while (*psubstr == *pstr) {
			psubstr++;
			pstr++;
			if (*psubstr == '\0') {
				return str;
			}
		}
		str++;
	}
	return NULL;
}