// Demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>


int value = 0;

void printvalue()
{
	printf("value = %d\n", value); //全局变量value 2
};


int main()
{
	int value = 0;

	value = 1;

	printf("value = %d\n", value); //局部变量value 1

	::value = 2; //::域操作符，全局变量value 2

	printvalue();

	return 0;

}
/**
 * value = 1
value = 2
 */