/*
 * V8_Base.cpp
 *
 *  Created on: 2016年1月20日
 *      Author: zhangyalei
 */

#include "V8_Base.h"
#include "Time_Value.h"
#include "Public_Define.h"

void Push_V8_Block(Block_Buffer *buf) {
	v8_data_list_.push_back(buf);
}

Block_Buffer* Pop_V8_Block() {
	Block_Buffer *buf = v8_data_list_.pop_front();
	if (buf) {
		return buf;
	} else {
		return nullptr;
	}
}

void read_int32(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Local<Object> obj = args.Holder();
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_int32();
	}
	args.GetReturnValue().Set(value);
}

void read_uint32(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Local<Object> obj = args.Holder();
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_uint32();
	}
	args.GetReturnValue().Set(value);
}

void read_uint16(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Local<Object> obj = args.Holder();
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_uint16();
	}
	args.GetReturnValue().Set(value);
}

void Get_Block(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(info.GetIsolate());
	localTemplate->SetInternalFieldCount(1);

	// Wrap the raw C++ pointer in an External so it can be referenced from within JavaScript.
	Block_Buffer *buf = Pop_V8_Block();
	if (buf) {
		Local<External> buf_ptr = External::New(info.GetIsolate(), buf);
		// Store the request pointer in the JavaScript wrapper.
		Local<Object> buf_obj = localTemplate->NewInstance(info.GetIsolate()->GetCurrentContext()).ToLocalChecked();
		buf_obj->SetInternalField(0, buf_ptr);

		// 为当前对象设置其对外函数接口
		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "read_int32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), read_int32)->GetFunction()) ;

		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "read_uint32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), read_uint32)->GetFunction()) ;

		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "read_uint16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), read_uint16)->GetFunction()) ;

		info.GetReturnValue().Set(buf_obj);
	} else {
		//设置对象为空
		info.GetReturnValue().SetNull();
	}
}

void Push_Game_Gate_Server_Block(int cid, Block_Buffer *buf) {
	GAME_GATE_SERVER->push_block(cid, buf);
}

void Sleep() {
	Time_Value::sleep(SLEEP_TIME);
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
  bool first = true;
  for (int i = 0; i < args.Length(); i++) {
    v8::HandleScope handle_scope(args.GetIsolate());
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    v8::String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    printf("%s", cstr);
  }
  printf("\n");
  fflush(stdout);
}

// The callback that is invoked by v8 whenever the JavaScript 'read'
// function is called.  This function loads the content of the file named in
// the argument into a JavaScript string.
void Read(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() != 1) {
    args.GetIsolate()->ThrowException(
        v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
                                v8::NewStringType::kNormal).ToLocalChecked());
    return;
  }
  v8::String::Utf8Value file(args[0]);
  if (*file == NULL) {
    args.GetIsolate()->ThrowException(
        v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                v8::NewStringType::kNormal).ToLocalChecked());
    return;
  }
  v8::Local<v8::String> source;
  if (!ReadFile(args.GetIsolate(), *file).ToLocal(&source)) {
    args.GetIsolate()->ThrowException(
        v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                v8::NewStringType::kNormal).ToLocalChecked());
    return;
  }
  args.GetReturnValue().Set(source);
}

// Reads a file into a v8 string.
v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const std::string& name) {
  FILE* file = fopen(name.c_str(), "rb");
  if (file == NULL) return v8::MaybeLocal<v8::String>();

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (size_t i = 0; i < size;) {
    i += fread(&chars[i], 1, size - i, file);
    if (ferror(file)) {
      fclose(file);
      return v8::MaybeLocal<v8::String>();
    }
  }
  fclose(file);
  v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
      isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
  delete[] chars;
  return result;
}

// The callback that is invoked by v8 whenever the JavaScript 'load'
// function is called.  Loads, compiles and executes its argument
// JavaScript file.
void Load(const v8::FunctionCallbackInfo<v8::Value>& args) {
  for (int i = 0; i < args.Length(); i++) {
    v8::HandleScope handle_scope(args.GetIsolate());
    v8::String::Utf8Value file(args[i]);
    if (*file == NULL) {
      args.GetIsolate()->ThrowException(
          v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                  v8::NewStringType::kNormal).ToLocalChecked());
      return;
    }
    v8::Local<v8::String> source;
    if (!ReadFile(args.GetIsolate(), *file).ToLocal(&source)) {
      args.GetIsolate()->ThrowException(
          v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                  v8::NewStringType::kNormal).ToLocalChecked());
      return;
    }
    if (!ExecuteString(args.GetIsolate(), source, args[i], false, false)) {
      args.GetIsolate()->ThrowException(
          v8::String::NewFromUtf8(args.GetIsolate(), "Error executing file",
                                  v8::NewStringType::kNormal).ToLocalChecked());
      return;
    }
  }
}

// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args) {
  // If not arguments are given args[0] will yield undefined which
  // converts to the integer value 0.
  int exit_code =
      args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
  fflush(stdout);
  fflush(stderr);
  exit(exit_code);
}

void Version(const v8::FunctionCallbackInfo<v8::Value>& args) {
  args.GetReturnValue().Set(
      v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion(),
                              v8::NewStringType::kNormal).ToLocalChecked());
}

// Executes a string within the current v8 context.
bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool print_result,
                   bool report_exceptions) {
  v8::HandleScope handle_scope(isolate);
  v8::TryCatch try_catch(isolate);
  v8::ScriptOrigin origin(name);
  v8::Local<v8::Context> context(isolate->GetCurrentContext());
  v8::Local<v8::Script> script;
  if (!v8::Script::Compile(context, source, &origin).ToLocal(&script)) {
    // Print errors that happened during compilation.
    if (report_exceptions)
      ReportException(isolate, &try_catch);
    return false;
  } else {
    v8::Local<v8::Value> result;
    if (!script->Run(context).ToLocal(&result)) {
      assert(try_catch.HasCaught());
      // Print errors that happened during execution.
      if (report_exceptions)
        ReportException(isolate, &try_catch);
      return false;
    } else {
      assert(!try_catch.HasCaught());
      if (print_result && !result->IsUndefined()) {
        // If all went well and the result wasn't undefined then print
        // the returned value.
        v8::String::Utf8Value str(result);
        const char* cstr = ToCString(str);
        printf("%s\n", cstr);
      }
      return true;
    }
  }
}

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
  v8::HandleScope handle_scope(isolate);
  v8::String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  v8::Local<v8::Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
    v8::Local<v8::Context> context(isolate->GetCurrentContext());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber(context).FromJust();
    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    v8::String::Utf8Value sourceline(
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
    v8::Local<v8::Value> stack_trace_string;
    if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
        stack_trace_string->IsString() &&
        v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {
      v8::String::Utf8Value stack_trace(stack_trace_string);
      const char* stack_trace_string = ToCString(stack_trace);
      fprintf(stderr, "%s\n", stack_trace_string);
    }
  }
}
