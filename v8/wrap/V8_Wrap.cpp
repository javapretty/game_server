/*
 * V8_Wrap.cpp
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Log.h"
#include "V8_Wrap.h"
#include "Buffer_Wrap.h"
#include "Player_Wrap.h"
#include "Time_Value.h"

Local<Context> Create_Context(Isolate* isolate) {
	Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
	global->Set(String::NewFromUtf8(isolate, "read", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, read));
	global->Set(String::NewFromUtf8(isolate, "require", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, require));
	global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, print));
	global->Set(String::NewFromUtf8(isolate, "sleep", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, sleep));
	global->Set(String::NewFromUtf8(isolate, "sec", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, sec));
	global->Set(String::NewFromUtf8(isolate, "msec", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, msec));
	global->Set(String::NewFromUtf8(isolate, "pop_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, pop_buffer));
	global->Set(String::NewFromUtf8(isolate, "push_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, push_buffer));
	global->Set(String::NewFromUtf8(isolate, "send_buffer_to_db", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, send_buffer_to_db));
	global->Set(String::NewFromUtf8(isolate, "process_login_buffer", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, process_login_buffer));
	global->Set(String::NewFromUtf8(isolate, "get_player_data", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player_data));
	global->Set(String::NewFromUtf8(isolate, "get_drop_player_cid", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_drop_player_cid));
	global->Set(String::NewFromUtf8(isolate, "get_player_by_cid", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player_by_cid));
	global->Set(String::NewFromUtf8(isolate, "get_player_by_name", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player_by_name));

	return Context::New(isolate, NULL, global);
}

const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

// Reads a file into a v8 string.
MaybeLocal<String> ReadFile(Isolate* isolate, const char* file_path) {
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

void ReportException(Isolate* isolate, TryCatch* try_catch) {
  HandleScope handle_scope(isolate);
  String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  Local<Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
    Local<Context> context(isolate->GetCurrentContext());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber(context).FromJust();
    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    String::Utf8Value sourceline(
        message->GetSourceLine(context).ToLocalChecked());
    const char* sourceline_string = ToCString(sourceline);
    fprintf(stderr, "%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn(context).FromJust();
    for (int i = 0; i < start; i++) {
      fprintf(stderr, " ");
    }
    int end = message->GetEndColumn(context).FromJust();
    for (int i = start; i < end; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "\n");
    Local<Value> stack_trace_string;
    if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
        stack_trace_string->IsString() &&
        Local<String>::Cast(stack_trace_string)->Length() > 0) {
      String::Utf8Value stack_trace(stack_trace_string);
      const char* stack_trace_string = ToCString(stack_trace);
      fprintf(stderr, "%s\n", stack_trace_string);
    }
  }
}

void read(const FunctionCallbackInfo<Value>& args) {
  if (args.Length() != 1) {
	  LOG_FATAL("read_file args error, length: %d\n", args.Length());
	  args.GetReturnValue().SetNull();
    return;
  	}
  String::Utf8Value file(args[0]);
  if (*file == NULL) {
	  LOG_FATAL("read_file:%s loading error\n");
	  	args.GetReturnValue().SetNull();
    return;
  	}

  Local<String> source;
  if (!ReadFile(args.GetIsolate(), *file).ToLocal(&source)) {
	  LOG_INFO("read_file:%s loading error\n", *file);
	  	args.GetReturnValue().SetNull();
    return;
  	}
  args.GetReturnValue().Set(source);
}

void require(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_INFO("require args error, length: %d\n", args.Length());
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
		LOG_INFO("script:%s not exist\n", file_path);
		return;
	}
  Local<Script> script = Script::Compile(isolate->GetCurrentContext(), source).ToLocalChecked();
  script->Run(isolate->GetCurrentContext()).ToLocalChecked();
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

void sleep(const FunctionCallbackInfo<Value>& args) {
	Time_Value::sleep(Time_Value(0,100));
}

void sec(const FunctionCallbackInfo<Value>& args) {
	double sec = Time_Value::gettimeofday().sec();
	args.GetReturnValue().Set(sec);
}

void msec(const FunctionCallbackInfo<Value>& args) {
	double msec = Time_Value::gettimeofday().msec();
	args.GetReturnValue().Set(msec);
}
