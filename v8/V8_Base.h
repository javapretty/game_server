/*
 * V8_Base.h
 *
 *  Created on: 2016年1月20日
 *      Author: zhangyalei
 */

#ifndef V8_BASE_H_
#define V8_BASE_H_

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"

const char* ToCString(const v8::String::Utf8Value& value);
void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
void Read(const v8::FunctionCallbackInfo<v8::Value>& args);
v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const std::string& name);
void Load(const v8::FunctionCallbackInfo<v8::Value>& args);
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args);
void Version(const v8::FunctionCallbackInfo<v8::Value>& args);
v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name);
bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool print_result,
                   bool report_exceptions);
void ReportException(v8::Isolate* isolate, v8::TryCatch* handler);


#endif /* V8_BASE_H_ */
