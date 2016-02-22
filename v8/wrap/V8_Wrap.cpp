/*
 * V8_Wrap.cpp
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "V8_Base.h"
#include "V8_Wrap.h"
#include "Buffer_Wrap.h"
#include "Player_Wrap.h"
#include "Time_Value.h"
#include "Public_Define.h"


Local<Context> create_v8_context(Isolate* isolate) {
	Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
	global->Set(String::NewFromUtf8(isolate, "require", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, require));
	global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, print));
	global->Set(String::NewFromUtf8(isolate, "sleep", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, sleep));
	global->Set(String::NewFromUtf8(isolate, "pop_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, pop_buffer));
	global->Set(String::NewFromUtf8(isolate, "push_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, push_buffer));
	global->Set(String::NewFromUtf8(isolate, "get_player_data_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player_data_buffer));
	global->Set(String::NewFromUtf8(isolate, "process_login_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, process_login_buffer));
	global->Set(String::NewFromUtf8(isolate, "get_player", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player));

	return Context::New(isolate, NULL, global);
}

void require(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		MSG_USER("require args wrong, length: %d\n", args.Length());
		return ;
	}

	String::Utf8Value str(args[0]);
	char file_path[128];
	sprintf(file_path, "js/%s", *str);
	run_script(args.GetIsolate(), file_path);
}

void run_script(Isolate* isolate, const char* file_path) {
	HandleScope handle_scope(isolate);
	Local<String> source;
	if (!ReadFile(isolate, file_path).ToLocal(&source)) {
		MSG_USER("script:%s not exist\n", file_path);
		return;
	}
  Local<Script> script = Script::Compile(isolate->GetCurrentContext(), source).ToLocalChecked();
  script->Run(isolate->GetCurrentContext()).ToLocalChecked();
}

void sleep(const FunctionCallbackInfo<Value>& args) {
	Time_Value::sleep(SLEEP_TIME);
}

void print(const FunctionCallbackInfo<Value>& args) {
  bool first = true;
  for (int i = 0; i < args.Length(); i++) {
    HandleScope handle_scope(args.GetIsolate());
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    printf("%s", cstr);
  }
  printf("\n");
  fflush(stdout);
}
