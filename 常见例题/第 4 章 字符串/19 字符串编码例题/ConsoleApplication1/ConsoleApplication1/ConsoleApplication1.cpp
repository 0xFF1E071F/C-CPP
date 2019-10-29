// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

char LowerCaseAlphabets[] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z'};

char UpperCaseAlphabets[] = {
	'A','B','C','D','E','F','G','H',
	'I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X',
	'Y','Z' };

void EncodeString(char* str, size_t len);
char GetFourthChar(char chrSrc, char *alphabets);

int main()
{
	char str[] = "hasd11H";
	printf(str);
	printf("\n");
	EncodeString(str, strlen(str));

	printf("encode string :%s\n", str);
	return 0;
}

void EncodeString(char* str, size_t len)
{
	//取+4
	for (size_t i = 0; i < len; i++) {
		if ('a' <= str[i] && str[i] <= 'z') {
			str[i] = GetFourthChar(str[i], LowerCaseAlphabets);
		}
		else if ('A' <= str[i] && str[i] <= 'Z') {
			str[i] = GetFourthChar(str[i], UpperCaseAlphabets);
		}
	}

	printf("%s\n", str);

	//翻转
	char* start = str;
	char* end = str + len - 1;
	while (start < end) {
		swap(*start, *end);
		start++;
		end--;
	}
}

char GetFourthChar(char chrSrc, char* alphabets)
{
	for (size_t i = 0; i < 26; i++) {
		if (chrSrc == alphabets[i]) {
			size_t index = (i + 4) % 26;
			return alphabets[index];
		}
	}
	return NULL;
}
