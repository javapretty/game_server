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

void read_int16(const FunctionCallbackInfo<Value>& args);
void read_int32(const FunctionCallbackInfo<Value>& args);
void Pop_Block(Local<String> property, const PropertyCallbackInfo<Value>& info);
void Push_Block(const FunctionCallbackInfo<Value>& args);
void Process_Login_Block(const FunctionCallbackInfo<Value>& args);

void Sleep(const FunctionCallbackInfo<Value>& args);
void Print(const FunctionCallbackInfo<Value>& args);

#endif /* V8_WRAP_H_ */
