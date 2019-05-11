// FuncPointer_Callback.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

//回调函数
void populate_array(int* array, size_t arraySize, /*返回值与函数指针间空格非必须;*/int(*getNextValue)(void))
{
	for (size_t i = 0; i < arraySize; i++) {
		array[i] = getNextValue();//调用函数获取生成的随机值并赋值
	}
}

//获取随机值
int getNextRandomValue(void)
{
	return rand();
}

int main()
{
	int myarray[10];

	populate_array(myarray, 10, getNextRandomValue);

	for (int i = 0; i < 10; i++) {
		printf("%d ", myarray[i]);
	}

	printf("\n");

	return 0;
}