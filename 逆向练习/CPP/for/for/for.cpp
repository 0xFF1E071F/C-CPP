#include "pch.h"
#include <iostream>
using namespace std;
//
//int main(int argc, char argv[])
//{
//	int num = 0;
//	int sum = 0;
//
//	for (num = 1; num <= 100; num++)
//	{
//		sum += num;
//	}
//	cout << "sum=" << sum << endl;
//
//	return 0;
//}

int main(int argc, char **argv)
{
	int ecx = 0;
	int edx = 0;
	int esi = 0;
	int eax = 1;
	int edi = 0;

	do
	{
		edi++;
		esi += 2;
		edx += 3;
		ecx += eax;
		edi += eax;
		esi += eax;
		edx += eax;
		eax += 4;
	} while (eax <= 0x64);



	// .text:00401027                 lea     eax, [edx+esi]
	//eax = (edx + esi);
	// .text:0040102A                 add     eax, edi
	//eax += edi;
	// .text:0040102C                 add     ecx, eax
	//ecx += eax;

	__asm
	{
		lea     eax, [edx + esi]
		add     eax, edi
		add     ecx, eax
	}

	// .text:0040102E                 push    offset sub_401360
	// .text:00401033                 push    ecx
	// .text:00401034                 mov     ecx, ds:?cout@std@@3V?$basic_ostream@DU?$char_traits@D@std@@@1@A ; std::basic_ostream<char,std::char_traits<char>> std::cout
	// .text:0040103A                 call    sub_401120
	// .text:0040103F                 mov     ecx, eax
	// .text:00401041                 call    ds:??6?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEAAV01@H@Z ; std::basic_ostream<char,std::char_traits<char>>::operator<<(int)
	// .text:00401047                 mov     ecx, eax
	// .text:00401049                 call    ds:??6?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEAAV01@P6AAAV01@AAV01@@Z@Z ; std::basic_ostream<char,std::char_traits<char>>::operator<<(std::basic_ostream<char,std::char_traits<char>> & (*)(std::basic_ostream<char,std::char_traits<char>> &))
	cout << "sum= " << ecx + edx + esi + edi << endl;

	// ida f5
	//while (v6 <= 100);
	//v8 = v7 + v4 + v5 + v3;  //这一段不知道咋计算的，参考ida的f5
	//v9 = sub_401120(std::cout);
	//v10 = std::basic_ostream<char, std::char_traits<char>>::operator<<(v9, v8, sub_401360);
	//std::basic_ostream<char, std::char_traits<char>>::operator<<(v10);


	return 0;
}