// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

int str2int(const char* str)
{
	int temp = 0;
	const char* ptr = str;

	if (*str == '-' || *str == '+') {
		str++;
	}

	while (*str != 0) {
		if (*str < '0' || *str > '9') {
			break;
		}
		temp = temp * 10 + (*str - '0');
		str++;
	}

	if (*ptr == '-') {
		temp = -temp;
	}

	return temp;
}

int main()
{
	int n = 0;
	char p[10] = "";

	cin.getline(p, 20);
	n = str2int(p);

	cout << n << endl;

	return 0;
}