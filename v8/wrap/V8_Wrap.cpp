/*
 * V8_Wrap.cpp
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include "V8_Wrap.h"
#include "Buffer_Wrap.h"
#include "Player_Wrap.h"
#include "Server_Wrap.h"
#include "Time_Value.h"
#include "Log.h"

Local<Context> Create_Context(Isolate* isolate) {
	Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
	global->Set(String::NewFromUtf8(isolate, "require", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, require));

	global->Set(String::NewFromUtf8(isolate, "read_json", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, read_json));

	global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, print));

	global->Set(String::NewFromUtf8(isolate, "sleep", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, sleep));

	global->Set(String::NewFromUtf8(isolate, "pop_game_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, pop_game_buffer));

	global->Set(String::NewFromUtf8(isolate, "push_game_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, push_game_buffer));

	//////////////////////////////////GameServer相关函数////////////////////////////////
	global->Set(String::NewFromUtf8(isolate, "register_game_timer", NewStringType::kNormal).ToLocalChecked(),
					FunctionTemplate::New(isolate, register_game_timer));

	global->Set(String::NewFromUtf8(isolate, "get_game_timer_id", NewStringType::kNormal).ToLocalChecked(),
				FunctionTemplate::New(isolate, get_game_timer_id));

	global->Set(String::NewFromUtf8(isolate, "send_game_buffer_to_db", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, send_game_buffer_to_db));

	global->Set(String::NewFromUtf8(isolate, "get_game_client_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_game_client_buffer));

	global->Set(String::NewFromUtf8(isolate, "push_game_client_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, push_game_client_buffer));

	global->Set(String::NewFromUtf8(isolate, "get_game_player_load_data_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_game_player_load_data_buffer));

	global->Set(String::NewFromUtf8(isolate, "get_drop_game_player_cid", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_drop_game_player_cid));

	global->Set(String::NewFromUtf8(isolate, "get_game_player_by_cid", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_game_player_by_cid));

	global->Set(String::NewFromUtf8(isolate, "get_game_player_by_name", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_game_player_by_name));

	//////////////////////////////////MasterServer相关函数////////////////////////////////
	global->Set(String::NewFromUtf8(isolate, "register_master_timer", NewStringType::kNormal).ToLocalChecked(),
					FunctionTemplate::New(isolate, register_master_timer));

	global->Set(String::NewFromUtf8(isolate, "get_master_timer_id", NewStringType::kNormal).ToLocalChecked(),
				FunctionTemplate::New(isolate, get_master_timer_id));

	global->Set(String::NewFromUtf8(isolate, "send_master_buffer_to_db", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, send_master_buffer_to_db));

	global->Set(String::NewFromUtf8(isolate, "get_master_client_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_master_client_buffer));

	global->Set(String::NewFromUtf8(isolate, "push_master_client_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, push_master_client_buffer));

	global->Set(String::NewFromUtf8(isolate, "get_master_player_load_data_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_master_player_load_data_buffer));

	global->Set(String::NewFromUtf8(isolate, "get_drop_master_player_cid", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_drop_master_player_cid));

	global->Set(String::NewFromUtf8(isolate, "get_master_player_by_cid", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_master_player_by_cid));

	global->Set(String::NewFromUtf8(isolate, "get_master_player_by_name", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_master_player_by_name));

	return Context::New(isolate, NULL, global);
}

const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string is null>";
}

int Run_Script(Isolate* isolate, const char* file_path) {
	HandleScope handle_scope(isolate);
	Local<String> source;
	if (!Read_File(isolate, file_path).ToLocal(&source)) {
		LOG_ERROR("read script:%s error\n", file_path);
		return -1;
	}

	Local<Context> context(isolate->GetCurrentContext());
	TryCatch try_catch(isolate);

	// Compile the script and check for errors.
	Local<Script> compiled_script;
	if (!Script::Compile(context, source).ToLocal(&compiled_script)) {
		Report_Exception(isolate, &try_catch, file_path);
		return -1;
	 }

	// Run the script!
	Local<Value> result;
	if (!compiled_script->Run(context).ToLocal(&result)) {
		Report_Exception(isolate, &try_catch, file_path);
		return -1;
	  }
  	return 0;
}

// Reads a file into a v8 string.
MaybeLocal<String> Read_File(Isolate* isolate, const char* file_path) {
  FILE* file = fopen(file_path, "rb");
  if (file == NULL) return MaybeLocal<String>();

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (size_t i = 0; i < size;) {
    i += fread(&chars[i], 1, size - i, file);
    if (ferror(file)) {
      fclose(file);
      return MaybeLocal<String>();
    }
  }
  fclose(file);
  MaybeLocal<String> result = String::NewFromUtf8(
      isolate, chars, NewStringType::kNormal, static_cast<int>(size));
  delete[] chars;
  return result;
}

void Report_Exception(Isolate* isolate, TryCatch* try_catch,  const char* file_path) {
  HandleScope handle_scope(isolate);
  String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  Local<Message> message = try_catch->Message();
  if (message.IsEmpty()) {
	  	LOG_ERROR("%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
	  Local<Context> context(isolate->GetCurrentContext());

    int linenum = message->GetLineNumber(context).FromJust();
    if (file_path == NULL) {
    		String::Utf8Value filename(message->GetScriptResourceName());
    		const char* filename_string = ToCString(filename);
    		LOG_ERROR("%s: %i: %s\n", filename_string, linenum, exception_string);
    	} else {
    		LOG_ERROR("%s: %i: %s\n", file_path, linenum, exception_string);
    	}

    // Print line of source code.
    String::Utf8Value sourceline(message->GetSourceLine(context).ToLocalChecked());
    const char* sourceline_string = ToCString(sourceline);
    LOG_ERROR("%s\n", sourceline_string);
    Local<Value> stack_trace_string;
    if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
        stack_trace_string->IsString() &&
        Local<String>::Cast(stack_trace_string)->Length() > 0) {
      String::Utf8Value stack_trace(stack_trace_string);
      const char* stack_trace_string = ToCString(stack_trace);
      LOG_ERROR("%s\n", stack_trace_string);
    }
  }
}

void read_json(const FunctionCallbackInfo<Value>& args) {
  if (args.Length() != 1) {
	  LOG_FATAL("read_json fatal, args length: %d\n", args.Length());
	  args.GetReturnValue().SetNull();
    return;
  	}
  String::Utf8Value file(args[0]);
  if (*file == NULL) {
	  LOG_FATAL("read_json fatal, file is null\n");
	  	args.GetReturnValue().SetNull();
    return;
  	}

  Local<String> source;
  if (!Read_File(args.GetIsolate(), *file).ToLocal(&source)) {
	  LOG_ERROR("read_file:%s fail\n", *file);
	  	args.GetReturnValue().SetNull();
    return;
  	}
  args.GetReturnValue().Set(source);
}

void require(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("require args error, length: %d\n", args.Length());
		return ;
	}

	String::Utf8Value str(args[0]);
	char file_path[128];
	sprintf(file_path, "js/%s", *str);
	Run_Script(args.GetIsolate(), file_path);
}

void print(const FunctionCallbackInfo<Value>& args) {
	std::ostringstream msg_stream;
  for (int i = 0; i < args.Length(); i++) {
	  HandleScope handle_scope(args.GetIsolate());
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    msg_stream << cstr;
  }
  LOG_DEBUG("%s", msg_stream.str().c_str());
}

void sleep(const FunctionCallbackInfo<Value>& args) {
	Time_Value::sleep(Time_Value(0,100));
}
