// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

void swapXor(int* a, int* b);
void swapLocalVar(int* a, int* b);
void swapArithmetic(int* a, int* b);

int main()
{
	int a1 = 1, b1 = 2;
	int a2 = 3, b2 = 4;
	int a3 = 5, b3 = 6;

	//异或方式交换变量的值
	printf("a1=%d, b1=%d\n", a1, b1);
	swapXor(&a1, &b1);
	printf("a1=%d, b1=%d\n", a1, b1);
	printf("-------------\n");
	/*
	a1=1, b1=2
	a1=2, b1=1
	*/

	//利用局部变量交换
	printf("a2=%d, b2=%d\n", a2, b2);
	swapLocalVar(&a2, &b2);
	printf("a2=%d, b2=%d\n", a2, b2);
	printf("-------------\n");

	//加减方式交换变量值
	printf("a3=%d, b3=%d\n", a3, b3);
	swapArithmetic(&a3, &b3);
	printf("a3=%d, b3=%d\n", a3, b3);
	printf("-------------\n");

	return 0;
}

/*
xor异或：两数相等为0，否则为1
*/
void swapXor(int* a, int* b)
{
	*b ^= *a;
	/*
	0001
	0010
	0011 b=3
	*/
	*a ^= *b;
	/*
	0001
	0011
	0010 a=2
	*/
	*b ^= *a;
	/*
	0011
	0010
	0001 b=1
	*/
}

void swapLocalVar(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void swapArithmetic(int* a, int* b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}
