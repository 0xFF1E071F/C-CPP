#pragma once


class CMyUI
{
public:
	CMyUI();
  ~CMyUI();
public:
  //一般错误消息提醒
  int Message(const char* szFmt, ...);
  //一般日志消息提醒 
  int Output(const char* szFmt, ...);
};

