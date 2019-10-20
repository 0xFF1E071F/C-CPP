// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：#ifdef、#else、#endif在程序中的使用

#include <stdio.h>
#include <stdlib.h>

#define DEBUG

int main()
{
	int i = 0;
	char c;

	while (1) {
		i++;
		c = getchar();
		if (c != '\n') {
			getchar();
		}
		if (c == 'q' || c == 'Q') {
#ifdef DEBUG
			printf("we got:%c, about to exit.\n", c);
#endif
			break;
		}
		else {
			printf("i = %d", i);
#ifdef DEBUG
			printf(", we got:%c", c);
#endif
			printf("\n");
		}
	}

	return 0;
}

/*
输入A：
输出：i = 1, we got:A

输入a:
输出：i = 2, we got:a

输入q/Q:
输出："we got:q, about to exit.
*/