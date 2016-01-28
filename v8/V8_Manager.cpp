/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include <iostream>
#include <sstream>
#include "V8_Manager.h"

V8_Manager::V8_Manager(void):platform_(nullptr), context_(nullptr) { }

V8_Manager::~V8_Manager(void) { }

V8_Manager *V8_Manager::instance_;

V8_Manager *V8_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new V8_Manager;
	return instance_;
}

void V8_Manager::run_handler(void) {
	init();
	start_v8();
}

int V8_Manager::init(void) {
	// Initialize V8.
	V8::InitializeICU();
	V8::InitializeExternalStartupData("");
	platform_ = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform_);
	V8::Initialize();
	context_ = new context;
	return 0;
}

int V8_Manager::fini(void) {
	// Dispose the isolate_ and tear down V8.
	delete context_;
	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform_;
	return 0;
}

int V8_Manager::start_v8() {
  wrap_block_buffer();

	v8::HandleScope scope(context_->isolate());
	Local<Value> result = context_->run_file("main.js");
	String::Utf8Value utf8(result);
	std::cout << *utf8 << std::endl;
  return 0;
}
