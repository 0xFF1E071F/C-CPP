// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

int main()
{
	char a;
	char* str1 = &a;
	char* str2 = (char*)"DDD";
	char str3[] = "AAA";

	//strcpy(str1, "hello");
	//cout << str1 << endl;

	//字符串常量，存储在静态存储区，不能被修改
	//str2[0] = 'B'; 
	//cout << str2 << endl;

	//要想修改字符，可使用字符数组，
	str3[0] = 'B';
	cout << str3 << endl;

	return 0;
}