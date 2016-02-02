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
#include "Block_List.h"
#include "Block_Buffer.h"

using namespace v8;

Local<Context> Create_V8_Context(Isolate* isolate);

void Sleep(const FunctionCallbackInfo<Value>& args);
void Print(const FunctionCallbackInfo<Value>& args);

void Read_Int16(const FunctionCallbackInfo<Value>& args);
void Read_Int32(const FunctionCallbackInfo<Value>& args);
void Pop_Block(const FunctionCallbackInfo<Value>& args);
void Push_Block(const FunctionCallbackInfo<Value>& args);
void Process_Login_Block(const FunctionCallbackInfo<Value>& args);

void Get_Player(const FunctionCallbackInfo<Value>& args);
void Respond_Success_Result(const FunctionCallbackInfo<Value>& args);
void Respond_Error_Result(const FunctionCallbackInfo<Value>& args);

#endif /* V8_WRAP_H_ */
