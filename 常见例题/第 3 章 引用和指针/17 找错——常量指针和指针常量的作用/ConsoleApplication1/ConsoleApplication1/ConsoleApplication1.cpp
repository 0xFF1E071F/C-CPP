// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int main()
{
	const char* node1 = "abc";
	char *const node2 = (char* const)"abc";

	printf("%d\n", &node1[2]);

	return 0;
}
