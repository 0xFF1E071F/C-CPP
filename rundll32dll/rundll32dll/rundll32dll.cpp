// rundll32dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

extern "C" _declspec(dllexport) 
void 
__cdecl 
rundll32dllfun(HWND hwnd,
			HINSTANCE hinst,
			LPSTR lpszCmdLine,
			int nCmdShow)
{
	MessageBoxA(NULL, "test", lpszCmdLine, MB_OK);
	return;
}



