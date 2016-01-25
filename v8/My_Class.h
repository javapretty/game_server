/*
 * My_Class.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef MY_CLASS_H_
#define MY_CLASS_H_

#include <string>

class MyClass
{
public:
	MyClass(int num);
	MyClass(const MyClass& rhs);
	virtual ~MyClass();

	int getNumber() const;
	void setNumber(int val);

	const char* getName() const;
	void setName(const char* name);

	void Method1();
	void Method2(const char* text);

private:
	int mNumber;
	std::string mName;
};

void myFunction();
void myFunction1(const char* text);
const char* myFunction2(int x);

#endif /* MY_CLASS_H_ */
