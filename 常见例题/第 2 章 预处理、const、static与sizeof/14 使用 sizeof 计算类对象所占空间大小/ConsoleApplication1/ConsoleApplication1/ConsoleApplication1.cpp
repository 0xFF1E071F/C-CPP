// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//考点：使用 sizeof 计算类对象所占空间大小
//请指出下面程序的输出是什么（在 32 位 WinNT 操作系统环境下）。

#include <iostream>
using namespace std;

class CEmpty {}; //空对象占1个字节

class A
{
public:
	int i; //4
};

class B
{
public:
	char ch; //1
};

class C
{
public:
	int i;   //4
	short j; //2
};		     //6，不能整除4，补2字节，8

class D
{
public:
	int i;    //4
	short j;  //2
	char ch;  //1
};            //7，不能整除4，补1字节，8

class E
{
public:
	int i;    //4
	int ii;   //4
	short j;  //2
	char ch;  //1
	char chr; //1
};            //12 /4 = 0

class F
{
public:
	int i;    //4
	int ii;   //4
	int iii;  //4
	short j;  //2
	char ch;  //1
	char chr; //1
};            //16 /4 = 0

int main()
{
	cout << "sizeof(int) = " << sizeof(int) << endl; //4
	cout << "sizeof(short) = " << sizeof(short) << endl; //2
	cout << "sizeof(char) = " << sizeof(char) << endl;//1
	cout << endl;
	cout << "sizeof(CEmpty) = " << sizeof(CEmpty) << endl; //0
	cout << "sizeof(A) = " << sizeof(A) << endl; //4
	cout << "sizeof(B) = " << sizeof(B) << endl; //1
	cout << "sizeof(C) = " << sizeof(C) << endl; //6
	cout << "sizeof(D) = " << sizeof(D) << endl; //7
	cout << "sizeof(E) = " << sizeof(E) << endl; //12
	cout << "sizeof(F) = " << sizeof(F) << endl; //16
	cout << endl;

	//空结构体
	struct SEmpty{};
	printf("%d\n", sizeof(struct SEmpty)); //1
	//练习1
	struct S1 {
		char c1; //1
		int i;   //填充7字节,8/4=0， 4
		char c2; //1
	};
	//(0)1......8...12
	printf("%d\n", sizeof(struct S1));//12

	//练习2 
	struct S2 {
		char c1; //1
		char c2; //1
		int i;   //填充5字节，4
	};
	//(0)11.....8
	printf("%d\n", sizeof(struct S2));//8

   //练习3 
	struct S3 {
		double d; //8
		char c;   //9
		int i;    //填充3字节，12/4=0, 4
	};
	//(0)1-(8)9...(11)12...16
	printf("%d\n", sizeof(struct S3));//16

	//练习4-结构体嵌套问题 
	struct S4 {
		char c1; //1，偏移0
		struct S3 s3; //S3结构体最大对齐值8，结构体整体对齐按8的整数倍
					  //偏移7位置处，加S3结构体自身大小16,24
		double d;  //8
	};
	//1......8...............24.......32
	printf("%d\n", sizeof(struct S4));//32

	return 0;
}

/*
sizeof(int) = 4
sizeof(short) = 2
sizeof(char) = 1

sizeof(CEmpty) = 1
sizeof(A) = 4
sizeof(B) = 1
sizeof(C) = 8
sizeof(D) = 8
sizeof(E) = 12
sizeof(F) = 16
*/

/*
注意：
字节对齐的细节和编译器实现相关，一般而言，需要满足 3 个准则：

1. 结构体变量的首地址能够被其最宽基本类型成员的大小所整除；

2. 结构体每个成员相对于结构体首地址的偏移量（offset）都是成员大小的整数倍，
   如有需要，编译器会在成员之间加上填充字节（internal adding）；

3. 结构体的总大小为结构体最宽基本类型成员大小的整数倍，
   如有需要，编译器会在最末一个成员之后加上填充字节（trailing padding）。

结构体的对齐规则：
1. 第一个成员在结构体变量偏移处为0的地址处。

2. 其他成员要对齐到某个数字（对齐数）的整数倍的地址处

	对齐数=编译器默认的一个对齐数与该成员大小的较小值。vs中默认为8。Linux中默认值为4。
3. 结构体总大小为最大对齐数的整数倍。

4. 如果嵌套了结构体的情况，嵌套的结构体对齐到自己的最大对齐数的整数倍处，
	结构体整体就是所有最大对齐数的整数倍。


修改对齐值：
方式1：
	/作用：C编译器将按照n个字节对齐。
	#pragma pack(8)//将默认对齐数改为8       
	//作用：取消自定义字节对齐方式。
	#pragma pack()//取消设置的默认对齐数，还原为默认  

---------------------------------
方式2：
	//作用：是指把原来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐
	#pragma pack(push)
	#pragma pack(2)     //这两行相当于#pragma pack(push, 2)
	//作用：恢复对齐状态
	#pragma pack(pop)

加入push和pop可以使对齐恢复到原来状态，而不是编译器默认，可以说后者更优，但是很多时候两者差别不大
*/
