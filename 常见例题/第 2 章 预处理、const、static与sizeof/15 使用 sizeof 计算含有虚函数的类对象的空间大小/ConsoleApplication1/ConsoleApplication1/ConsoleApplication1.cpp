// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//考点：使用 sizeof 计算含有虚函数的类对象的空间大小
//请指出下面程序的输出是什么（在 32 位 WinNT 操作系统环境下）。

#include <iostream>
using namespace std;

class Base
{
public:
	Base(int x) :a(x)
	{

	}

	void print()
	{
		cout << "base" << endl;
	}
private:
	int a;
};

class Derived :public Base
{
public:
	Derived(int x) :Base(x - 1), b(x)
	{

	}

	void print()
	{
		cout << "derived" << endl;
	}
private:
	int b;
};

class A
{
public:
	A(int x) :a(x)
	{

	}
	virtual void print()
	{
		cout << "A" << endl;
	}

	virtual void log()
	{
		cout << "A log" << endl;
	}
private:
	int a;
};

class B : public A
{
public:
	B(int x) :A(x - 1), b(x)
	{
		ch = 'c';
		chr = 'h';
	}
	virtual void print()
	{
		cout << "B" << endl;
	}

private:
	char ch;
	int b;
	char chr;
	
};

int main()
{
	Base obj1(1);
	cout << "size of Base obj is " << sizeof(obj1) << endl;
	Derived obj2(2);
	cout << "size of Derived obj is " << sizeof(obj2) << endl;

	A a(1);
	cout << "size of A obj is " << sizeof(a) << endl;
	B b(2);
	cout << "size of B obj is " << sizeof(b) << endl;
	
}

/*
size of Base obj is 4
size of Derived obj is 8
size of A obj is 8
size of B obj is 20
*/

/*
普通函数不占用内存，只要有虚函数，就会占用一个指针大小的内存，
原因是系统多用了一个指针维护这个类的虚函数表，
并且注意这个虚函数无论含有多少项（类中含有多少个虚函数）都不会再影响类的大小。

计算对象占用内存大小，同样要注意数据成员对齐值的问题

虚表指针：先自己的，再父类的
数据成员：先父类的，再自己的
*/
