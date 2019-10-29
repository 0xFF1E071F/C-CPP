// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <iostream>
int main()
{
	char* str[] = { (char*)"Welcome", (char*)"to", (char*)"Fortemedia", (char*)"Nanjing" };// ABCD
	char** p = str + 1; //p指向B, 
	str[0] = (*p++) + 2;//*p取值，p++,p指向C，p + 2，指向D后一个元素（空）
	str[1] = *(p + 1);  //p未移动，+1取D位置元素，Nanjing
	str[2] = p[1] + 3;  //p[1]指向D， +3向后数3个字符，jing
	str[3] = p[0] + (str[2] - str[1]); //p[0]指向jing ,+3 g
	printf("%s\n", str[0]);
	printf("%s\n", str[1]);
	printf("%s\n", str[2]);
	printf("%s\n", str[3]);
	/*

	Nanjing
	jing
	g
	*/

	//int x = 3;
	//int* p, * q;
	//p = &x, q = &x;
	//printf("%d\n", *p++);    // ++优先级高于*，按理应该先p++再*p，但++为后加，先*p再p++
	//printf("%d\n", x);       // 3
	//printf("%d\n", (*q)++);  // *q先取出，然后对*q做++运行
	//printf("%d\n", x);       // 4
	//printf("%d\n", *(q++));  // *q先取出，再对q++
	//printf("%d\n", x);       // 4
	/*
	3
	3
	3
	4
	4
	4
	*/

	return 0;
}

