/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "V8_Manager.h"
#include "V8_Base.h"
#include "V8_Wrap.h"

V8_Manager::V8_Manager(void):platform_(nullptr), isolate_(nullptr) { }

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
	//初始化V8
	V8::InitializeICU();
	V8::InitializeExternalStartupData("");
	platform_ = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform_);
	V8::Initialize();

	ArrayBufferAllocator allocator;
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = &allocator;
	isolate_ = Isolate::New(create_params);

	start_v8();
	return 0;
}

int V8_Manager::fini(void) {
	//释放V8资源
	isolate_->LowMemoryNotification();
	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform_;
	return 0;
}

int V8_Manager::start_v8() {
	//进入V8作用域
	Isolate::Scope isolate_scope(isolate_);
	HandleScope scope(isolate_);
	//创建V8执行环境
	Local<Context> context = Create_V8_Context(isolate_);
	Context::Scope context_scope(context);
	Local<String> source;
	if (!ReadFile(isolate_, "main.js").ToLocal(&source)) {
		MSG_USER("Error reading main.js.\n");
	  return 1;
	}
  Local<Script> script = Script::Compile(context, source).ToLocalChecked();
  script->Run(context).ToLocalChecked();

  return 0;
}
