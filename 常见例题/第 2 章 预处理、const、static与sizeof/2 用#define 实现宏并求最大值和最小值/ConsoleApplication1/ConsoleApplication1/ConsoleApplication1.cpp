// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：#define 宏定义的使用

#include <iostream>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main()
{
	printf("%d and %d ,max is %d\n", 1, 2, MAX(1, 2));
	printf("%d and %d ,min is %d\n", 1, 2, MIN(1, 2));
	/*
	1 and 2 ,max is 2
	1 and 2 ,min is 1
	*/
	
	return 0;
}

/*
1. #define 宏定义的使用
2. 三目运算符(?:)的使用
3. 宏定义的参数要用括号括起来，宏只是简单的文本替换，一定要注意这一点
*/
