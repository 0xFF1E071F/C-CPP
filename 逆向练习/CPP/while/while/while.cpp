// while.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	int num = 1;
	int sum = 0;
	while (num <= 100)
	{
		sum += num;
		num++;
	}
	cout << "sum=" << sum << endl;
	return 0;
}


// 同for循环，被优化为do while