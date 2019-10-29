// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

void LoopMove(char* str, int n);

int main()
{
	char str[] = "123456789";
	int steps = 0;
	printf("str :%s\n", str);
	printf("input step: ");
	scanf("%d", &steps);
	LoopMove(str, steps); //向右循环移位
	printf("after LoopMove %d: %s\n",steps, str);
}

void LoopMove(char* str, int n)
{
	char* head = str;
	char* tmp = (char*)malloc(n);//临时保存从尾部移到头部的字符串
	int nlen = strlen(str);
	int i = 0;
	if (tmp == NULL) {
		return;
	}
	memset(tmp, 0, n);
	for (; i < n; i++) {
		tmp[i] = head[nlen - n + i];
	}
	//从头部移到尾部，从后面开始移
	for (i = nlen - 1; i >= n; i--) {
		head[i] = head[i - n];
	}
	//把尾部字符写到头部
	for (i = 0; i < n; i++) {
		head[i] = tmp[i];
	}
	free(tmp);
}