// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>

char getChar(int x, int y)
{
	char c;

	unsigned int a = x; 

	(a + y > 10) ? (c = 1) : (c = 2);

	/************************************************************************/
	/*
	当表达式中存在有符号类型和无符号类型时，所有的操作数都自动转换成无符号类型。
	y如果是负数，会转化成很大的无符号正数
	*/
	/************************************************************************/
	return c;
}

int main(void)
{
	char c1 = getChar(7, 4); //1

	char c2 = getChar(7, 3); //2

	char c3 = getChar(7, -7); //2

	char c4 = getChar(7, -8); //1

	printf("c1 = %d\n", c1);

	printf("c2 = %d\n", c2);

	printf("c3 = %d\n", c3);

	printf("c4 = %d\n", c4);

	return 0;
}