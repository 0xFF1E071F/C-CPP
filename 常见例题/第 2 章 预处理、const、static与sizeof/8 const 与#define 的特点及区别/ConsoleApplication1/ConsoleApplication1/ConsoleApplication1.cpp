// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：对 const 与#define 的特点及区别的理解

#include <iostream>

#define PI 3.1415926

int main()
{
	float angel;
	angel = 30 * PI / 180;
	printf("angel = %f\n", angel);
	return 0;
}

/*
#define 
当程序编译的时候，
编译器会首先将#define PI 3.14251926以后所有代码中的PI替换为3.1415926，
然后再编译。

#define 常量是一个Compile-Time概念，它的生命周期止于编译期，
它存在于程序的代码段，
在实际程序中它只是一个常数、一个命令中的参数，并没有实际的存在。


const常量存在于程序的数据段，并在堆栈中分配了空间。
const常量是一个Run-Time概念，它在程序中确确实实存在着并可以被调用、传递。

const常量有数据类型而宏常量没有数据类型。
编译器可以对const常量进行类型安全检查。

*/
