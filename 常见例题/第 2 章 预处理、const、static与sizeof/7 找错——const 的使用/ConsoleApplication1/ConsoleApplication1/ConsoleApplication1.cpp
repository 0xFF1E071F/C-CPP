// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：const 使用时的注意点

#include <stdio.h>

int main()
{
	const int x = 1;
	int b = 10;
	int c = 20;

	const int* a1 = &b;
	int* const a2 = &b;
	const int* const a3 = &b;

	//x = 2; 
	//x是整型常量不能修改
	//要修改整型常量需要去掉const属性

	a1 = &c;
	//a1定义的类型为const int* a1,const int* 在它的左侧，
	//修饰的是指针指向的变量,即指针指向的是常量
	//修改指针a1本身是允许的。
	//*a1 = 1;
	//a1指针指向常量，不能修改值

	//a2 = &c;
	//int* const a2, const在int*的右侧，修饰指针，指针本身是常量
	*a2 = 1;

	//a3 = &c;
	//*a3 = 1;
	//const int* const a3, a3指针本身是常量，指针指向的变量也是常量

	return 0;
}

/*
读的时候，按老外风格就是先读左边再读右边
const int* a1 ： a1 是int类型指针指向const类型的变量，变量是常量
int* const a2 ： a2 是const int类型的指针，指针自身是常量
const int* const a3： a3 是const int类型的指针，指向const类型的变量
*/