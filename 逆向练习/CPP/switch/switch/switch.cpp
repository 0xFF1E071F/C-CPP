#include "pch.h"
#include<iostream>
using namespace std;
int main()
{
	int num = 0;
	cin >> num;
	switch (num)
	{
		case 1:
		{
			cout << "1" << endl;
			break;
		}
		case 2:
		{
			cout << "2" << endl;
			break;
		}
		case 3:
		{
			cout << "3" << endl;
			break;
		}
		case 4:
		{
			cout << "4" << endl;
			break;
		}
		default:
		{
			cout << "default" << endl;
			break;
		}
	}
	return 0;
}