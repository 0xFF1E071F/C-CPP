// TestStrConvertDec.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <windows.h>
#include <iostream>

BOOL StrConvertDec(char *pSrc, unsigned int &nDec)
{
  int nLen = strlen(pSrc);
  unsigned int nSum = 0;

  if (nLen > 8)
  {
    nDec = NULL;
    return FALSE;
  }

  for (int i = nLen - 1, j = 0; i >= 0; i--, j++)
  {
    if (pSrc[i] >= '0'&&pSrc[i] <= '9')
    {
      nSum += (pSrc[i] - '0')*(unsigned int)pow(16, j);
      continue;
    }

    if (pSrc[i] >= 'a'&&pSrc[i] <= 'f')
    {
      nSum += (pSrc[i] - 'a' + 10)*(unsigned int)pow(16, j);
      continue;
    }

    if (pSrc[i] >= 'A'&&pSrc[i] <= 'F')
    {
      nSum += (pSrc[i] - 'A' + 10)*(unsigned int)pow(16, j);
      continue;
    }

    //nDec = NULL;
    //return FALSE;
  }

  nDec = nSum;
  return TRUE;

}

int main(int argc, char* argv[])
{
  char szSrc[] = "0x403000";
  unsigned int nDec = 0;

  StrConvertDec(szSrc, nDec);

  printf("%08X ", nDec);

  return 0;
}