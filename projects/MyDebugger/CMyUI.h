#pragma once


class CMyUI
{
public:
	CMyUI();
  ~CMyUI();
public:
  //һ�������Ϣ����
  int Message(const char* szFmt, ...);
  //һ����־��Ϣ���� 
  int Output(const char* szFmt, ...);
};

