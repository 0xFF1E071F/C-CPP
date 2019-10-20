// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

class A{};

class B{};

class C :public A, public B {};

class D:virtual public A{}; //C虚继承A，C中分配指向A类对象的指针

class E:virtual public A, virtual public B{};
//E虚继承A和B，E中分配指向A类对象的指针和B类对象的指针

class F
{
public:
	int a;
	static int b;
	/*
	类F含有一个静态成员变量，这个静态成员的空间不在类的实例中，
	而是像全局变量一样在静态存储区中，被每一个类的实例共享。
	*/
};

int F::b = 10;


int main()
{
	cout << "sizeof(A) = " << sizeof(A) << endl; //1
	cout << "sizeof(B) = " << sizeof(B) << endl; //1
	cout << "sizeof(C) = " << sizeof(C) << endl; //1
	cout << "sizeof(D) = " << sizeof(D) << endl; //4
	cout << "sizeof(E) = " << sizeof(E) << endl; //8
	cout << "sizeof(F) = " << sizeof(F) << endl; //8

	return 0;
}