// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：使用 sizeof 计算普通变量所占空间大小
//假设在 32 位 WinNT 操作系统环境下

#include <iostream>

void Func(char str[100]);

int main()
{
	char str[] = "Hello";
	char* p1 = str;
	int n = 10;
	

	printf("sizeof(str) : %d\n", sizeof(str)); // strlen(str)+1, 6
	printf("sizeof(p1) : %d\n", sizeof(p1)); //4
	/*
	指针都是地址，即不管是char*还是int*在32位机中，指针都是4字节，
	char和int表示的是该指针所指向的地址对数据的解析方式
	*/

	printf("sizeof(n) : %d\n", sizeof(n)); //4
	
	Func(str);//传递指针

	void* p2 = malloc(100);
	printf("sizeof(p2) : %d\n", sizeof(p2)); //4

	return 0;
}

void Func(char str[100])
{
	printf("Func sizeof(str) : %d\n", sizeof(str));//4
}
/*
sizeof(str) : 6
sizeof(p1) : 4
sizeof(n) : 4
Func sizeof(str) : 4
sizeof(p2) : 4
*/