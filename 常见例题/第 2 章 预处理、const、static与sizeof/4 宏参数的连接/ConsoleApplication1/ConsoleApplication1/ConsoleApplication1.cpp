// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：如何连接宏参数

#include <stdio.h>

#define STR(s) #s
#define CONS(a,b) (int)(a##e##b)

int main()
{
	printf(STR(vck));
	printf("\n");
	printf("%d\n", CONS(2, 3));

	/*
	vck
	整型值 2e3 => 十进制 2000
	*/
	return 0;
}