// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

class widget
{
public:
	widget()
	{
		count++;
	}
	~widget()
	{
		--count;
	}
	static int num()
	{
		return count;
	}
private:
	static int count;
};

int widget::count = 0;

int main()
{
	widget x, y; //2个对象，执行2次构造
	cout << "The Num is " << widget::num() << endl; //2
	if (widget::num() > 1)
	{
		widget x, y, z; //3个对象，执行3次构造
		cout << "The Num is " << widget::num() << endl; //5
	}
	//出作用域，执行3次析构
	widget z; //1个对象，执行1次构造
	cout << "The Num is " << widget::num() << endl; //3

	return 0;
}

/*
The Num is 2
The Num is 5
The Num is 3
*/