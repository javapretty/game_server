/*
 * object_wrap.cpp
 *
 *  Created on: 2016年1月26日
 *      Author: zhangyalei
 */

#include <iostream>
#include "object_wrap.h"

using namespace std;

MyClass::MyClass(int num)
{
	cout << "Constructor of MyClass" << endl;
	mNumber = num;
}

MyClass::MyClass(const MyClass& rhs)
{
	cout << "Copy constructor of MyClass" << endl;
	mNumber = rhs.mNumber;
	mName = rhs.mName;
}

MyClass::~MyClass()
{
	cout << "Destructor of MyClass" << endl;
}

int MyClass::getNumber() const
{
	return mNumber;
}

void MyClass::setNumber(int val)
{
	mNumber = val;
}

const char* MyClass::getName() const
{
	return mName.c_str();
}

void MyClass::setName(const char* name)
{
	mName = name;
}

void MyClass::Method1()
{
	cout << "method1" << endl;
}

void MyClass::Method2(const char* text)
{
	cout << "method2 " << text << endl;
}

void myFunction()
{
	cout << "This is my function!" << endl;
}

void myFunction1(const char* text)
{
	cout << "This is my function 1: " << text << endl;
}

const char* myFunction2(int x)
{
	cout << "This is my function 2: " << x << endl;

	return "This is result of function 2";
}
