// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：sizeof不能用于计算字符串长度

#include <iostream>
using namespace std;

//void UpperCase(char str[])
//{
//	int test = sizeof(str);
//	int test2 = sizeof(str[0]);
//
//	for (size_t i = 0; i < sizeof(str) / sizeof(str[0]); i++)
//	{
//		if ('a' <= str[i] && str[i] <= 'z')
//		{
//			str[i] -= ('a' - 'A');
//		}
//	}
//}

void UpperCase(char str[], size_t nlen)
{
	for (size_t i = 0; i < nlen; i++)
	{
		if ('a' <= str[i] && str[i] <= 'z')
		{
			str[i] -= ('a' - 'A');
		}
	}
}

void UpperCase2(char str[])
{
	for (size_t i = 0; i <strlen(str); i++)
	{
		if ('a' <= str[i] && str[i] <= 'z')
		{
			str[i] -= ('a' - 'A');
		}
	}
}

void LowerCase(char str[], size_t nlen)
{
	for (size_t i = 0; i < nlen; i++)
	{
		if ('A' <= str[i] && str[i] <= 'Z')
		{
			str[i] -= ('A' - 'a');
		}
	}
}

int main()
{
	char str[] = "aBcDe";
	//cout << "The length of str is " << sizeof(str) / sizeof(str[0]) << endl;
	cout << "The length of str is " << strlen(str) << endl;
	//UpperCase(str, strlen(str));
	UpperCase2(str);
	cout << str << endl;

	LowerCase(str, strlen(str));
	cout << str << endl;

	return 0;
}

/*
小写字母转大写字母，小减大
	'a'<= && <= 'z'， x -= ('a' - 'A')
大写字母转小写字母，大减小
	'A'<= && <= 'Z'，x -= ('A' - 'a')
*/