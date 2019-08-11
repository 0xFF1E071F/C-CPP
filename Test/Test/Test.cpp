// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

void foo(int * a)
{
	*a = 5;
}

int main()
{
	int a = 0;

	foo(&a);

	std::cout << a;
	//char SegmentTypes[][256] = {
	//"<Reserved>",//Data Read-Only缩写是：Data RO，也可认为是： <Reserved>。如果结构（UINT64）全部为零，也可认为是Reserved。
	//"Data RO AC",//Data Read-Only, accessed
	//"Data RW",//Data Read/Write
	//"Data RW AC",//Data Read/Write, accessed
	//"Data RO ED",//Data Read-Only, expand-down
	//"Data RO ED AC",//Data Read-Only, expand-down, accessed
	//   "Data RW ED",//Data Read/Write, expand-down
	//   "Data RW ED AC",//Data Read/Write, expand-down, accessed

	//   "Code EO",//Code Execute-Only
	//   "Code EO AC",//Code Execute-Only, accessed
	//   "Code RE",//Code Execute/Read 加空格以便显示的对齐。
	//   "Code RE AC",//Code Execute/Read, accessed
	//   "Code EO CO",//Code Execute-Only, conforming
	//   "Code EO CO AC",//Code Execute-Only, conforming, accessed
	//   "Code RE CO",//Code Execute/Read, conforming
	//   "Code RE CO AC",//Code Execute/Read, conforming, accessed
	//   "TSS32 Busy ",//这个也可显示只要识别了TSS及内容。
	//   "TSS32 Avl" //这个在X86上出现了。
	//}; 
	//
	////std::cout << SegmentTypes[1] << std::endl;
	//std::cout << sizeof(INT) << std::endl;
	//std::cout << strlen(SegmentTypes[1]) << std::endl;
	//std::cout << sizeof(SegmentTypes[1]) << std::endl;

	//class A {};
	//A a;
	//std::cout << sizeof(A);

	//std::cout << sizeof(ULONG_PTR);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
