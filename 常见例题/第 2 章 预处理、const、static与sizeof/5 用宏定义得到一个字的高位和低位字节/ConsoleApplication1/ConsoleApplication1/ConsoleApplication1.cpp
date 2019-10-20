// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//考点：宏定义与位运算的使用

#include <stdio.h>
//#include <Windows.h>
//WORD

#define WORD_HI(x) ((unsigned char)((unsigned short)(x) >> 8))
#define WORD_LO(x) ((unsigned char)((unsigned short)(x) & 0xff))

int main()
{
	int n = 0xDA82;
	printf("WORD_HI: %X(%d)\n", WORD_HI(n), WORD_HI(n)); //DA(218)
	printf("WORD_LO: %X(%d)\n", WORD_LO(n), WORD_LO(n)); //82(130)
	/*
	WORD_HI: DA(218)
	WORD_LO: 82(130)
	*/
	return 0;
}
