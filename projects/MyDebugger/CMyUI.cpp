#include "CmyUI.h"
#include <varargs.h>
#include <stdio.h>
#include <stdarg.h>
#include <WINDOWS.H>

CMyUI::CMyUI()
{

}

CMyUI::~CMyUI()
{
}

int CMyUI::Message(const char* szFmt, ...)
{
  char buffer[260] = { 0 };

  va_list args;
  va_start(args, szFmt);
  int result = vsnprintf(buffer, sizeof(buffer), szFmt, args);
  va_end(args);
  //MessageBoxA(NULL, buffer, "SuerperDebug", MB_OK);
  printf(buffer);
  return 0;
}

int CMyUI::Output(const char* szFmt, ...)
{
  va_list args;
  va_start(args, szFmt);
  int result = vprintf(szFmt, args);
  va_end(args);
  return result;
  return 0;
}
