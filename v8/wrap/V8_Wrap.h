/*
 * V8_Wrap.h
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#ifndef V8_WRAP_H_
#define V8_WRAP_H_

#include "include/v8.h"
#include "include/libplatform/libplatform.h"

using namespace v8;

Local<Context> Create_Context(Isolate* isolate);
void replace_string(char* str, char old_char, char new_char);
const char* ToCString(const String::Utf8Value& value);
MaybeLocal<String> ReadFile(Isolate* isolate, const char* file_path);
void ReportException(Isolate* isolate, TryCatch* handler);

void read(const FunctionCallbackInfo<Value>& args);
void require(const FunctionCallbackInfo<Value>& args);
void run_script(Isolate* isolate, const char* file_path);
void load_json_file(const FunctionCallbackInfo<Value>& args);
void print(const FunctionCallbackInfo<Value>& args);
void sleep(const FunctionCallbackInfo<Value>& args);
void sec(const FunctionCallbackInfo<Value>& args);
void msec(const FunctionCallbackInfo<Value>& args);

#endif /* V8_WRAP_H_ */
