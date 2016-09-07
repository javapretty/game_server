/*
 * V8_Manager.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: zhangyalei
 */

#include "Log.h"
#include "V8_Manager.h"
#include "V8_Wrap.h"
#include "Server_Config.h"
#include "Public_Struct.h"

V8_Manager::V8_Manager(void):platform_(nullptr), isolate_(nullptr) { }

V8_Manager::~V8_Manager(void) {
	fini();
}

int V8_Manager::init(const char *server_path) {
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
	Run_Script(isolate_, server_misc[server_path].asCString());
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
	init("game_server_path");
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
	//多进程模式开启master v8虚拟机
	if (SERVER_CONFIG->server_misc()["server_type"].asInt() == MULTI_PROCESS)	{
		init("master_server_path");
	}
}
