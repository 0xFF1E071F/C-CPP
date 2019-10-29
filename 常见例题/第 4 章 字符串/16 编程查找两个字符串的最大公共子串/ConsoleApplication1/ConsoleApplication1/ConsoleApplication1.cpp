// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <iostream>

char* commonString(const char* str1, const char* str2)
{
	char *substr = NULL;
	char *longstr = NULL;
	char* shortstr = NULL;
	if (str1 == NULL || str2 == NULL) {
		return NULL;
	}
	if (strlen(str1) > strlen(str2)) {
		longstr = (char*)str1;
		shortstr = (char*)str2;
	}
	else {
		longstr = (char*)str2;
		shortstr = (char*)str1;
	}
	if (strstr(longstr, shortstr) != NULL) {
		return shortstr;
	}
	size_t i = 0, j = 0;
	//申请堆空间存放返回结果
	substr = (char*)malloc(sizeof(char) * (strlen(shortstr) + 1));
	for (i = strlen(shortstr) - 1; i > 0; i--) {
		for (j = 0; j <= strlen(shortstr) - i; j++) {
			memcpy(substr, &shortstr[j], i);
			substr[i] = '\0'; //i是拷贝的长度，逐渐减小
			if (strstr(longstr, substr) != NULL) {
				return substr;
			}
		}
	}
	return NULL;
}

int main()
{
	char *str1 = (char*)malloc(256);
	char *str2 = (char*)malloc(256);
	char* substr = NULL;
	fgets(str1, 255, stdin);
	fgets(str2, 255, stdin);
	substr = commonString(str1, str2);
	printf("%s\n", substr);
	free(substr);
	free(str1);
	free(str2);
	return 0;
}
