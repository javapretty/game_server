/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"
#include "V8_Manager.h"
#include "V8_Class.h"
#include "V8_Property.h"
#include "test.h"

using namespace v8;

V8_Manager::V8_Manager(void):context_(nullptr) { }

V8_Manager::~V8_Manager(void) { }

V8_Manager *V8_Manager::instance_;

V8_Manager *V8_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new V8_Manager;
	return instance_;
}

void V8_Manager::run_handler(void) {
	start_v8();
}

int V8_Manager::start_v8() {
  // Initialize V8.
  V8::InitializeICU();
  V8::InitializeExternalStartupData("");
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  context_ = new context;

  v8_wrap_class();
  //test_v8_wrap();

  // Dispose the isolate_ and tear down V8.
  delete context_;
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;

  return 0;
}

struct X
{
	int var = 1;

	int get() const { return var; }
	void set(int v) { var = v; }

	void fun1() { std::cout << var << std::endl; }
	void fun2() { std::cout << var+1 << std::endl; }
};

int V8_Manager::v8_wrap_class(void) {
	//context context;
	v8::Isolate* isolate = context_->isolate();
	v8::HandleScope scope(isolate);

	class_<X> X_class(isolate);
	X_class
		.ctor()
		.set("var", &X::var)
		.set("get", property(&X::get))
		.set("set", &X::set)
		.set("fun1", &X::fun1)
		.set("fun2", &X::fun2)
	;

	context_->set("X", X_class);
	Local<Value> result = context_->run_file("wrap.js");
	String::Utf8Value utf8(result);
	std::cout << *utf8 << std::endl;

	return 0;
}

void V8_Manager::test_v8_wrap()
{
	test_utility();
	test_convert();
	test_call_func();
	test_function();
	test_factory();
	test_module();
	test_class();
	test_object();
	test_myclass();
}
