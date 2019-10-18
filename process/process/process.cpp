// process.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <shellapi.h>

int main()
{
	//ShellExecute(NULL, "open", "calc.exe", NULL, NULL, SW_SHOWMAXIMIZED);
	ShellExecute(NULL, "open", "C:\\Users\\SystemSearch.exe", NULL, NULL, SW_SHOWMAXIMIZED);

	return 0;

	//STARTUPINFO st;
	//memset(&st, 0, sizeof(st));
	//	//启动窗口的信息
	//PROCESS_INFORMATION pi; //进程的信息
	//memset(&pi, 0, sizeof(pi));

	////TCHAR cmdline[256] = TEXT("notepad.exe");
	//TCHAR cmdline[256] = TEXT("test.exe");
	//BOOL bRet = CreateProcess(NULL,
	//	cmdline,
	//	NULL, 
	//	NULL,
	//	FALSE,
	//	NORMAL_PRIORITY_CLASS, 
	//	NULL, 
	//	NULL, 
	//	&st, 
	//	&pi);

	//if (!bRet)
	//	printf("%d", GetLastError());
	//	return 0;
}
