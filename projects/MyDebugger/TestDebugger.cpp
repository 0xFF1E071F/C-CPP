// TestDebugger.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "CMyDbg.h"
#include <iostream>

//打开被调试程序
int OpenDbgExe(const char* pszFilePath)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = (char*)pszFilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "Exe Files(*.exe)\0*.exe\0All Files(*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;

	if (GetOpenFileName(&ofn))
		return 0;
	else
		return -1;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
}

int main(int argc, char* argv[])
{
	char szFilePath[MAX_PATH] = "";
	
	if (OpenDbgExe(szFilePath) == 0)
	{
		CMyDbg dbg;
		BOOL bRet = FALSE;
		//PE文件解析
		bRet = dbg.DbgInit(szFilePath);
		if (bRet)
		{
			//启动调试进程
			dbg.DbgStartProcess(szFilePath);
		}

	}

	system("pause");
	return 0;
}