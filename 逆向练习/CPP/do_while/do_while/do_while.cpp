// do_while.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

//int main(int argc, char **argv)
//{
//	int num = 1;
//	int sum = 0;
//	do
//	{
//		sum += num;
//		num++;
//	} while (num <= 100);
//
//	cout << "sum=" << sum;
//
//	return 0;
//}

// .text:00401000 ; int __cdecl main(int argc, const char **argv, const char **envp)
int main(int argc, const char **argv, const char **envp)
{
// .text:00401000 main            proc near               ; CODE XREF: start-8D↓p
// .text:00401000
// .text:00401000 argc            = dword ptr  4
// .text:00401000 argv            = dword ptr  8
// .text:00401000 envp            = dword ptr  0Ch
// .text:00401000
// .text:00401000                 mov     eax, 1
	int eax = 1;
// .text:00401005                 xor     ecx, ecx
	int ecx = 0;
// .text:00401007
// .text:00401007 loc_401007:                             ; CODE XREF: main+D↓j
	do
	{

// .text:00401007                 add     ecx, eax
		ecx += eax;
// .text:00401009                 inc     eax
		eax++;
	} while (eax <= 0x64);
// .text:0040100A                 cmp     eax, 64h
// .text:0040100D                 jle     short loc_401007

// .text:0040100F                 push    ecx
// .text:00401010                 mov     ecx, ds:?cout@std@@3V?$basic_ostream@DU?$char_traits@D@std@@@1@A ; std::basic_ostream<char,std::char_traits<char>> std::cout
// .text:00401016                 call    sub_4010F0
// .text:0040101B                 mov     ecx, eax
// .text:0040101D                 call    ds:??6?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEAAV01@H@Z ; std::basic_ostream<char,std::char_traits<char>>::operator<<(int)
	cout << "sum=" << ecx;

	return 0;
// .text:00401023                 xor     eax, eax
// .text:00401025                 retn
}
// .text:00401025 main            endp