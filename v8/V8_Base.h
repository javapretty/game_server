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
#include "Game_Server.h"
#include "Block_List.h"
#include "Block_Buffer.h"

////使用全局变量保存数据
typedef Block_List<Thread_Mutex> Data_List;
static Data_List v8_data_list_;

void Push_V8_Block(Block_Buffer *buf);
Block_Buffer* Pop_V8_Block();
void Push_Game_Gate_Server_Block(int cid, Block_Buffer *buf);
void Sleep();

const char* ToCString(const v8::String::Utf8Value& value);
void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
void Read(const v8::FunctionCallbackInfo<v8::Value>& args);
v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const std::string& name);
void Load(const v8::FunctionCallbackInfo<v8::Value>& args);
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args);
void Version(const v8::FunctionCallbackInfo<v8::Value>& args);
bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool print_result,
                   bool report_exceptions);
void ReportException(v8::Isolate* isolate, v8::TryCatch* handler);


#endif /* V8_BASE_H_ */
