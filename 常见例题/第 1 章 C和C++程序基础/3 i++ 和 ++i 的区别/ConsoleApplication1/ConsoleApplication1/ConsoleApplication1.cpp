// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>

int main(void)
{
	int i = 8;

	printf("%d\n", ++i); //i先自增1,再打印，输出9

	printf("%d\n", --i);//i先自减1，再打印，输出8

	printf("%d\n", i++);//先打印i，输出8，i再自加1,9

	printf("%d\n", i--);//先打印i,输出9，i再自减1,8

	printf("%d\n", -i++);//先打印-i,输出-8，i再自加1，9

	printf("%d\n", -i--);//先打印-i，输出-9，i再自减1，8

	printf("------\n");

	return 0;
}
/**
 * 9
8
8
9
-8
-9
------

 */