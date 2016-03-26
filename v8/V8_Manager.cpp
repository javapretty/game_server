/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "V8_Manager.h"
#include "V8_Wrap.h"
#include "Buffer_Wrap.h"
#include "Server_Config.h"
#include "Public_Struct.h"

V8_Manager::V8_Manager(void):platform_(nullptr), isolate_(nullptr) { }

V8_Manager::~V8_Manager(void) { }

V8_Manager *V8_Manager::instance_;

V8_Manager *V8_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new V8_Manager;
	return instance_;
}

void V8_Manager::run_handler(void) { }

int V8_Manager::init(int server_type) {
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
	//进入v8的Isolate内部，才能使用V8引擎
	Isolate::Scope isolate_scope(isolate_);
	//创建V8执行环境
	HandleScope handle_scope(isolate_);
	Local<Context> context = Create_Context(isolate_);
	context_.Reset(isolate_, context);
	//进入V8执行环境内部
	Context::Scope context_scope(context);
	//根据不同的服务器加载不同的脚本
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_type == GAME_SERVER) {
		Run_Script(isolate_, server_misc["game_server_js_path"].asCString());
	} else if (server_type == MASTER_SERVER) {
		Run_Script(isolate_, server_misc["master_server_js_path"].asCString());
	} else {
		LOG_FATAL("server_type:%d error", server_type);
	}
	return 0;
}

int V8_Manager::fini(void) {
	//释放V8资源
	isolate_->LowMemoryNotification();
	context_.Reset();
	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform_;
	return 0;
}

int V8_Manager::js_load_player_data(Block_Buffer *buf) {
	//执行V8代码前，必须先进入V8的Isolate，初始化V8运行环境
	Isolate::Scope isolate_scope(isolate_);
	HandleScope handle_scope(isolate_);
	Local<Context> context = Local<Context>::New(isolate_, context_);
	Context::Scope context_scope(context);

	//获取js函数
	Local<String> func_name = String::NewFromUtf8(isolate_, "load_data", NewStringType::kNormal).ToLocalChecked();
	Local<Value> func_value;
	if (!context->Global()->Get(context, func_name).ToLocal(&func_value) || !func_value->IsFunction()) {
	    return -1;
	 }
	//转换成js函数对象
	Local<Function> js_func = Local<Function>::Cast(func_value);

	// Invoke the process function, giving the global object as 'this'
	TryCatch try_catch(isolate_);
	Local<Object> buf_obj = wrap_buffer(isolate_, buf);
	const int argc = 1;
	Local<Value> argv[argc] = {buf_obj};
	Local<Value> result;
	if (!js_func->Call(context, context->Global(), argc, argv).ToLocal(&result)) {
		String::Utf8Value error(try_catch.Exception());
	  printf("js_load_player_data error, : %s", *error);
		return -1;
	}
	return 0;
}
