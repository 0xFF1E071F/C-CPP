// FuncPointer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//声明一个指向同样参数、返回值的函数指针类型
//typedef int (*fun_ptr)(int, int);
//typedef int(*fun_ptr)(int, int);

#include <iostream>

int max(int x, int y)
{
	return x > y ? x : y;
}

int main()
{
	//函数指针,返回值与函数指针之前可以不加空格
	int (*p)(int, int) = &max;
	int(*p1)(int, int) = &max;

	int (*p2)(int, int) = max;
	int(*p3)(int, int) = max;
    
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	printf("请输入三个数字：");
	scanf_s("%d %d %d", &a, &b, &c);

	//p，p1函数指针调用方式
	//d = p1(p(a, b), c);

	//p2,p3函数指针调用方式
	d = (*p3)((*p2)(a,b), c);

	printf("最大的数字是：%d\n", d);

	return 0;
}