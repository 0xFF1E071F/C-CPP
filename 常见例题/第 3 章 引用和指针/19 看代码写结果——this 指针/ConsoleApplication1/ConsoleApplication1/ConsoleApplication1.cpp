// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

class MyClass
{
public:
	int data;
	MyClass(int data)
	{
		this->data = data;
	}
	void print()
	{
		cout << data << endl; //this->data;
		cout << "hello!" << endl;
	}
};

int main()
{
	MyClass* pMyClass;
	pMyClass = new MyClass(1);
	pMyClass->print();  //1 hello!
	pMyClass[0].print();//1 hello!
	pMyClass[1].print(); //
	pMyClass[100000].print(); //

	return 0;
}

/*
hello!
hello!
hello!
hello!
*/

/*
1
hello!
1
hello!
-33686019
hello!
段错误
*/