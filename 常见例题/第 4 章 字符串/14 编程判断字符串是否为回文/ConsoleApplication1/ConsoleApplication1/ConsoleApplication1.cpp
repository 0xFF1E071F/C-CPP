// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

size_t IsRevStr(const char* str)
{
	size_t nfound = 1;
	size_t nlen = strlen(str);
	for (size_t i = 0; i < nlen/2; i++) {
		if (*(str + i) != *(str + nlen - i - 1)) {
			nfound = 0;
			break;
		}
	}
	return nfound;
}

int main()
{
	char str[] = "1234321";
	char str2[] = "124321";
	size_t nret = 0;
	nret = IsRevStr(str);
	printf("%d\n", nret);
	nret = IsRevStr(str2);
	printf("%d\n", nret);
}
