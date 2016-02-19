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

using namespace v8;

const char* ToCString(const String::Utf8Value& value);
void Read(const FunctionCallbackInfo<Value>& args);
MaybeLocal<String> ReadFile(Isolate* isolate, const char* file_path);
void Load(const FunctionCallbackInfo<Value>& args);
bool ExecuteString(Isolate* isolate, Local<String> source,
                   Local<Value> name, bool print_result,
                   bool report_exceptions);
void ReportException(Isolate* isolate, TryCatch* handler);


#endif /* V8_BASE_H_ */
