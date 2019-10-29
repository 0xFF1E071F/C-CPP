// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <assert.h>

void* mymemcpy(void* dst,
	const void* src,
	size_t length);

int main()
{
	char strSrc[] = "Hello World!";
	char strDst[20] = { '\0' };
	printf("sizeof(strSrc):%d\n", sizeof(strSrc));
	printf("strlen(strSrc):%d\n", strlen(strSrc));
	mymemcpy(strDst, strSrc, strlen(strSrc));
	printf("%s\n", strDst);
	return 0;
}

void* mymemcpy(void* dst,
	const void* src,
	size_t length)
{
	//dst和src必须有效
	assert(dst != NULL && src != NULL);
	char* pdst = (char*)dst;
	char* psrc = (char*)src;
	while (length > 0) {
		*pdst = *psrc;
		length--;
		pdst++;
		psrc++;
	}
	return dst;
}

