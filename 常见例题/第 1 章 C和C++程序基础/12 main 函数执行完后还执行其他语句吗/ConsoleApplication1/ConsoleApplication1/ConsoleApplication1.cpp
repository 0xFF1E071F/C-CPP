// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// 考点：atexit()函数的使用

#include <stdio.h>
#include <stdlib.h>

void fn1();
void fn2();

int main()
{
	atexit(fn1);
	atexit(fn2);
	printf("call main...\n");
	/*
	call main...
	call fn2 ...
	call fn1 ...
	*/

	return 0;
}

void fn1()
{
	printf("call fn1 ...\n");
}

void fn2()
{
	printf("call fn2 ...\n");
}

/*
总结：
atexit函数用来注册程序退出后执行的函数，
atexit函数原型，无参无返回值，
先注册的函数，后调用，与注册顺序相反。
*/