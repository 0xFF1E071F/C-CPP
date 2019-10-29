// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int mystrcmp(const char* src, const char* dst);

int main()
{
	char src[] = "hello";
	char dst[] = "world";
	int ret = mystrcmp(src, dst);
	printf("mystrcmp ret is:%d\n", ret);

	char src2[] = "hello";
	char dst2[] = "hello";
	ret = mystrcmp(src2, dst2);
	printf("mystrcmp ret is:%d\n", ret);

	char src3[] = "hello";
	char dst3[] = "ddddd";
	ret = mystrcmp(src3, dst3);
	printf("mystrcmp ret is:%d\n", ret);
}

int mystrcmp(const char* src, const char* dst)
{
	int ret = 0;
	//src == dst , 0
	//src > dst, 1
	//src < dst, -1
	//循环比较两个字符串，如果不等或者遇到dst字符串末尾，则退出循环
	for (;;) {
		ret = *(unsigned char*)src - *(unsigned char*)dst;
		if (!ret && *dst) { //ret == 0
			++src;
			++dst;
			continue;
		}
		//ret 1或-1
		break;
	}
	if (ret > 0) {
		ret = 1;
	}
	else if (ret < 0) {
		ret = -1;
	}
	else {
		ret = 0;
	}
	return ret;
}