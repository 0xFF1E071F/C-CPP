// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

void revstr(char* src);

int main()
{
	char src[] = "I am from Shanghai";
	cout << src << endl;
	revstr(src);
	cout << src << endl;
	return 0;
}

void revstr(char* src)
{
	char *start = src, *end = src, *ptr = src;
	//while (*ptr != '\0') {
	//	ptr++;       //==> start = src, end = ptr - 1;
	while (*ptr++ != '\0') { //==> start = src, end = ptr - 2;
		//找到一个单词
		if (*ptr == ' ' || *ptr == '\0') {
			//end指向单词末尾
			end = ptr - 1;
			while (start < end) {
				//把单词字母逆序
				swap(*start++, *end--);
			}
			//指向下一个单词开头
			start = end = ptr + 1;
		}
	}
	//start指向字符串开头，end指向字符串末尾
	//start = src, end = ptr - 2;
	start = src, end = ptr - 2;
	while (start < end) {
		swap(*start++, *end--);
	}	
}