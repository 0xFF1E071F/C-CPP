#include "pch.h"
#include <iostream>
#include <Windows.h>

int main(int argc, char *argv[])
{
	//MessageBox(NULL, TEXT("hello auto start"), TEXT("Title"), MB_OK);

	printf("this exe is called by other exe\r\n");

	system("pause");

	return 0;
}
