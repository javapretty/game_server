/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include <iostream>
#include <sstream>
#include "V8_Manager.h"
#include "V8_Base.h"

V8_Manager::V8_Manager(void):platform_(nullptr), isolate_(nullptr), context_(nullptr) { }

V8_Manager::~V8_Manager(void) { }

V8_Manager *V8_Manager::instance_;

V8_Manager *V8_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new V8_Manager;
	return instance_;
}

void V8_Manager::run_handler(void) {
	init();
}

int V8_Manager::init(void) {
	// Initialize V8.
	V8::InitializeICU();
	V8::InitializeExternalStartupData("");
	platform_ = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform_);
	V8::Initialize();

	start_v8_context();
	//start_v8_base();
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

int V8_Manager::start_v8_base() {
	ArrayBufferAllocator allocator;
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = &allocator;
	isolate_ = Isolate::New(create_params);

	Isolate::Scope isolate_scope(isolate_);
	HandleScope scope(isolate_);

	Local<ObjectTemplate> global = ObjectTemplate::New(isolate_);
	global->SetInternalFieldCount(1);
	global->Set(String::NewFromUtf8(isolate_, "Print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate_, Print));
	global->SetAccessor(
		String::NewFromUtf8(isolate_, "Get_Block", NewStringType::kInternalized)
		.ToLocalChecked(),
		Get_Block);

	Local<Context> context = Context::New(isolate_, NULL, global);
	global_context_.Reset(isolate_, context);

	//wrap_pointer(global);

	Context::Scope context_scope(context);
	Local<String> source;
	if (!ReadFile(isolate_, "main.js").ToLocal(&source)) {
	  fprintf(stderr, "Error reading main.js.\n");
	  return 1;
	}
  Local<Script> script = Script::Compile(context, source).ToLocalChecked();
  Local<Value> result = script->Run(context).ToLocalChecked();
	String::Utf8Value utf8(result);
	std::cout << *utf8 << std::endl;

	isolate_->LowMemoryNotification();
  return 0;
}

int V8_Manager::start_v8_context(void) {
	context_ = new context;
	v8::HandleScope scope(context_->isolate());

	wrap_block();

	Local<Value> result = context_->run_file("main.js");
	String::Utf8Value utf8(result);
	std::cout << *utf8 << std::endl;
	return 0;
}
