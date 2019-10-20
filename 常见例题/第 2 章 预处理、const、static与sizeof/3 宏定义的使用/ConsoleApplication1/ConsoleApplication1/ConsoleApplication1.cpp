// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：使用#define 宏定义时需要注意的地方

#include <stdio.h>

#define SQR(x) (x*x)

int main()
{
	int a, b = 3;
	a = SQR(b + 2);
	printf("a = %d\n", a);
	return 0;
}

/*
注意：
宏展开是在预处理时期，也就是编译之前，
此时11行b并没有被赋值，b只是一个符号，
#define SQR(x) (x*x)
11行被展开成
a = (b+2 * b+2);
  = (3+2*3+2)
  = 11

要执行预期平方计算，需要修改宏定义的参数，加上括号
#define SQR(x) ((x)*(x))
11行会展开成
a = ((b+2)*(b+2))
  = (5 * 5)
  = 25
*/
