// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

void int2str(int n, char* str)
{
	char buf[10] = "";
	int i = 0;
	int len = 0;
	int temp = n < 0 ? -n : n;
	//整数个位数+'0'=char字符
	while (temp) {
		buf[i] = (temp % 10) + '0';
		temp = temp / 10;
		i++;
	}
	//处理-负号的情况，负号多一位长度
	len = n < 0 ? ++i : i;
	str[i] = 0;
	//反向存放
	while (1) {
		i--; //-1是忽略'\0'字符，最后一位放'\0'字符
		if (buf[len - i - 1] == 0) {
			break;
		}
		str[i] = buf[len - i - 1]; //-1是忽略'\0'字符，str逆序赋值
	}
	if (i == 0) {
		str[i] = '-';
	}
}

int main()
{
	int n = -1234;
	char str[10];

	int2str(n, str);

	printf("str %s\n", str);

	return 0;
}
