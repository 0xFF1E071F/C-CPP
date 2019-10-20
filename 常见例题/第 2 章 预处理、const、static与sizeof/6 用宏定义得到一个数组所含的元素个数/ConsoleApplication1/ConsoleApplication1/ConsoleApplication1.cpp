// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

//考点：宏定义与 sizeof 的使用

#define ARR_SIZE(x) (sizeof((x)) / sizeof((x[0])))

int main()
{
	int nAry[] = { 1, 2, 3, 4, 5, 6 };
	printf("nAry elements num is %d\n", ARR_SIZE(nAry));
	/*
	nAry elements num is 6
	*/
	return 0;
}