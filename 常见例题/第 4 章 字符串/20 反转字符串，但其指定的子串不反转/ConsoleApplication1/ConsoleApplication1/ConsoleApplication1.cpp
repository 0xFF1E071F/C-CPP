// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stack>
using namespace std;

const char* reverseString(const char* str, const char* token)
{
	stack<char> stkChar;
	const char* ptoken = token;
	const char* head = str;
	const char* rear = str;
	while (*head != '\0') {
		//找到子串token
		while (*head != '\0' && *ptoken == *head) {
			ptoken++;
			head++;
		}
		//子串倒序压入栈
		if (*ptoken == '\0') {
			const char* p;
			for (p = head - 1; p >= rear; p--) {
				stkChar.push(*p);
			}
			ptoken = token;
			rear = head;//指定新的起始位置，从子串后开始
		}
		else { //不是子串直接压入栈
			stkChar.push(*rear);
			rear++;
			ptoken = token;
			head = rear;//指定新的起始位置，从子串后开始
		}
	}
	char* pReturn = new char[strlen(str) + 1];
	size_t i = 0;
	while (!stkChar.empty()) {
		pReturn[i] = stkChar.top();
		stkChar.pop();
		i++;
	}
	pReturn[i] = '\0';
	return pReturn;
}

int main()
{
	char welcome[] = "Welcome you, my friend";
	printf("%s\n", welcome);
	char token[] = "you";
	const char* pReverse = reverseString(welcome, token);
	printf("%s\n", pReverse);
	delete[] pReverse;
	pReverse = NULL;
	return 0;
}
