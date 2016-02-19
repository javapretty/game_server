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
#include "Block_Buffer.h"

using namespace v8;

Local<Context> create_v8_context(Isolate* isolate);

void require(const FunctionCallbackInfo<Value>& args);
void run_script(Isolate* isolate, const char* file_path);
void sleep(const FunctionCallbackInfo<Value>& args);
void print(const FunctionCallbackInfo<Value>& args);

void process_login_block(const FunctionCallbackInfo<Value>& args);
void get_player(const FunctionCallbackInfo<Value>& args);
void respond_success_result(const FunctionCallbackInfo<Value>& args);
void respond_error_result(const FunctionCallbackInfo<Value>& args);
void role_id(const FunctionCallbackInfo<Value>& args);
void role_name(const FunctionCallbackInfo<Value>& args);

#endif /* V8_WRAP_H_ */
