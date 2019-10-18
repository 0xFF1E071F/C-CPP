// TestALT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <afx.h>
#include <atldef.h>
#include <atlconv.h>
#include <atlbase.h> 
#include <atlstr.h> 

int main(int argc, char *argv[])
{
	USES_CONVERSION_EX;
	CString cTemp = _T("char to cstring");
	char * pChar = T2A_EX(cTemp);

	return 0;
}
