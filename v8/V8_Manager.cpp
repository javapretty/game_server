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

V8_Manager::V8_Manager(void):platform_(nullptr), isolate_(nullptr) { }

V8_Manager::~V8_Manager(void) {
	fini();
}

int V8_Manager::init(int server_type) {
	//初始化V8
	V8::InitializeICU();
	V8::InitializeExternalStartupData("");
	platform_ = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform_);
	V8::Initialize();

	//初始化v8虚拟机
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
	if (server_type == 1) {
		Run_Script(isolate_, server_misc["game_server_js_path"].asCString());
	} else if (server_type == 2) {
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

//////////////////////////////////////////////////////////////////////////////////////////
Game_V8_Manager::Game_V8_Manager(void) { }

Game_V8_Manager::~Game_V8_Manager(void) { }

Game_V8_Manager *Game_V8_Manager::instance_ = 0;

Game_V8_Manager *Game_V8_Manager::instance(void) {
	if (! instance_)
		instance_ = new Game_V8_Manager;
	return instance_;
}

void Game_V8_Manager::run_handler(void) {
	init(1);
}

//////////////////////////////////////////////////////////////////////////////////////////
Master_V8_Manager::Master_V8_Manager(void) { }

Master_V8_Manager::~Master_V8_Manager(void) { }

Master_V8_Manager *Master_V8_Manager::instance_ = 0;

Master_V8_Manager *Master_V8_Manager::instance(void) {
	if (! instance_)
		instance_ = new Master_V8_Manager;
	return instance_;
}

void Master_V8_Manager::run_handler(void) {
	init(2);
}
