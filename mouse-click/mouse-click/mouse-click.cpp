// mouse-click.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <windows.h>

//constexpr auto VK_LWIN = 91;
constexpr auto VK_R = 83;
//constexpr auto lpszPath = TEXT("C:\\Users\\SystemSearch.exe");
constexpr auto lpszPath = TEXT("C:\\test.exe");

int main()
{
	// Win + R
	::keybd_event(VK_LWIN, 0, 00, 0);
	::keybd_event('R', 0, 00, 0);
	::keybd_event('R', 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);

	HWND hwnd = ::FindWindow(NULL, TEXT("运行"));
	for (int i = 0; i < 200; i++)
	{
		if (hwnd)
			break;
		Sleep(1000);
		hwnd = ::FindWindow(NULL, TEXT("运行"));
	}
	if (!hwnd)
	{
		exit(1);
	}

	HWND hCob = ::FindWindowEx(hwnd, NULL, TEXT("ComboBox"), NULL);
	for (int i = 0; i < 200; i++)
	{
		if (hCob)
			break;
		Sleep(1000);
		hCob = ::FindWindowEx(hwnd, NULL, TEXT("ComboBox"), NULL);
	}
	if (!hCob)
	{
		exit(1);
	}

	HWND hEdit = ::FindWindowEx(hCob, NULL, TEXT("Edit"), NULL);
	for (int i = 0; i < 200; i++)
	{
		if (hEdit)
			break;
		Sleep(1000);
		hEdit = ::FindWindowEx(hCob, NULL, TEXT("Edit"), NULL);
	}
	if (!hEdit)
	{
		exit(1);
	}
	SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszPath);


	HWND hSrue = ::FindWindowEx(hwnd, NULL, TEXT("Button"), TEXT("确定"));
	for (int i = 0; i < 200; i++)
	{
		if (hSrue)
			break;
		Sleep(1000);
		hSrue = ::FindWindowEx(hwnd, NULL, TEXT("Button"), TEXT("确定"));
	}
	if (!hSrue)
	{
		exit(1);
	}

	// 点击
	::SendMessage(hSrue, WM_LBUTTONDOWN, 0, NULL);
	// 弹起
	::SendMessage(hSrue, WM_LBUTTONUP, 0, NULL);

	return 0;
}
