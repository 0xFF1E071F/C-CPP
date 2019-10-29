// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

char* addBigInt(char* num1, char* num2);

int main()
{
	char num1[] = "123456789323";
	char num2[] = "45671254563123";
	char* result = NULL;
	result = addBigInt(num1, num2);
	//如果结果最高位不等于 0，则输出结果
	if (result[0] != '0') {
		printf("%s + %s = %s\n", num1, num2, result);
	}
	else {
		printf("%s + %s = %s\n", num1, num2, result+1);
	}
	//释放内存
	free(result);
	result = NULL;	
	return 0;
}

char* addBigInt(char* num1, char* num2)
{
	int c = 0;//进位，开始最低位为0
	int i = strlen(num1) - 1;//指向第一个加数的最低位
	int j = strlen(num2) - 1;//指向第二个加数的最低位
	
	int maxLength = strlen(num1) >= strlen(num2) ?
		strlen(num1) + 1 : strlen(num2) + 1;//得到2个数中较大数的位数
	char* ret = (char*)malloc(maxLength + 1); //保存结果
	int k;
	if (ret == NULL) {
		printf("malloc error!\n");
		exit(1);
	}
	ret[maxLength] = '\0';
	k = strlen(ret) - 1;//指向结果数组的最低位
	//同位数计算
	while (i >= 0 && j >= 0) {
		//计算本位的值
		ret[k] = ((num1[i] - '0') + (num2[j] - '0') + c) % 10 + '0';
		//向高位进位值
		c = ((num1[i] - '0') + (num2[j] - '0') + c) / 10;
		--i;
		--j;
		--k;
	}
	//计算剩余位数
	while (i >= 0) {
		//计算本位值
		ret[k] = ((num1[i] - '0') + c) % 10 + '0';
		//向高位进位值
		c = ((num1[i] - '0') + c) / 10;
		--i;
		--k;
	}
	while (j >= 0) {
		//计算本位值
		ret[k] = ((num2[j] - '0') + c) % 10 + '0';
		//向高位进位值
		c = ((num2[j] - '0') + c) / 10;
		--j;
		--k;
	}
	ret[0] = c + '0';
	//如果结果最高位不等0，则输出结果
	//if (ret[0] != '0') {
	//	return ret;
	//}
	//else {
	//	return ret + 1;
	//}
	return ret;
}
