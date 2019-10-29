// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

int main()
{
	int x1[4] = { 1, 2, 3, 4 };
	int x2[2] = { 5, 6 };
	int x3[3] = { 7, 8, 9 };
	int* a[2];
	int* b = x1;
	int i = 0;

	a[0] = x2;
	a[1] = x3;

	cout << "输出a[0]:";
	for (i = 0; i < sizeof(x2) / sizeof(int); i++) {
		cout << a[0][i] << " ";
	}
	cout << endl;

	cout << "输出a[1]:";
	for (i = 0; i < sizeof(x3) / sizeof(int); i++) {
		cout << a[1][i] << " ";
	}
	cout << endl;

	cout << "输出b:";
	for (i = 0; i < sizeof(x1) / sizeof(int); i++) {
		cout << b[i] << " ";
	}
	cout << endl;

	return 0;
}