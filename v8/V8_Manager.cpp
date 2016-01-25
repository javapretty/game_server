/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "My_Class_Template.h"
#include "V8_Base.h"
#include "V8_Manager.h"
#include "test.h"

V8_Manager::V8_Manager(void):isolate_(0) { }

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

  // Create a new Isolate and make it the current one.
  ArrayBufferAllocator allocator;
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = &allocator;
  isolate_ = Isolate::New(create_params);

  process_script();
  test_v8pp();

  // Dispose the isolate_ and tear down V8.
  isolate_->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;

  return 0;
}

int V8_Manager::process_script(void) {
	Isolate::Scope isolate__scope(isolate_);

	// Create a stack-allocated handle scope.
	HandleScope handle_scope(isolate_);

	// Create a new context.
	// Local<Context> context = Context::New(isolate_);
	Local<Context> context = CreateShellContext(isolate_);

	// Enter the context for compiling and running the hello world script.
	Context::Scope context_scope(context);

	// Create a string containing the JavaScript source code.
	//Local<String> source =	String::NewFromUtf8(isolate_, jsCode, NewStringType::kNormal).ToLocalChecked();
	Local<String> source;
	if (!ReadFile(isolate_, "test.js").ToLocal(&source)) {
		fprintf(stderr, "Error reading test.js.\n");
		return 1;
	}

	// Compile the source code.
	Local<Script> script = Script::Compile(context, source).ToLocalChecked();

	// Run the script to get the result.
	Local<Value> result = script->Run(context).ToLocalChecked();

	// Convert the result to an UTF8 string and print it.
	String::Utf8Value utf8(result);
	printf("%s\n", *utf8);

	isolate_->LowMemoryNotification();
	UnregisterAll();
	return 0;
}

void V8_Manager::test_v8pp()
{
	test_utility();
	test_context();
	test_convert();
	test_call_func();
	test_function();
	test_factory();
	test_module();
	test_class();
	test_property();
	test_object();
}
