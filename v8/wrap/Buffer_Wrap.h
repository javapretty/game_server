/*
 * Block_Wrap.h
 *
 *  Created on: 2016年2月19日
 *      Author: zhangyalei
 */

#ifndef BLOCK_WRAP_H_
#define BLOCK_WRAP_H_

#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "Block_Buffer.h"

using namespace v8;

Local<Object> wrap_buffer(Isolate* isolate, Block_Buffer *buf);
Block_Buffer *unwrap_buffer(Local<Object> obj);

void read_int8(const FunctionCallbackInfo<Value>& args);
void read_int16(const FunctionCallbackInfo<Value>& args);
void read_int32(const FunctionCallbackInfo<Value>& args);
void read_int64(const FunctionCallbackInfo<Value>& args);
void read_uint8(const FunctionCallbackInfo<Value>& args);
void read_uint16(const FunctionCallbackInfo<Value>& args);
void read_uint32(const FunctionCallbackInfo<Value>& args);
void read_uint64(const FunctionCallbackInfo<Value>& args);
void read_double(const FunctionCallbackInfo<Value>& args);
void read_bool(const FunctionCallbackInfo<Value>& args);
void read_string(const FunctionCallbackInfo<Value>& args);

void write_int8(const FunctionCallbackInfo<Value>& args);
void write_int16(const FunctionCallbackInfo<Value>& args);
void write_int32(const FunctionCallbackInfo<Value>& args);
void write_int64(const FunctionCallbackInfo<Value>& args);
void write_uint8(const FunctionCallbackInfo<Value>& args);
void write_uint16(const FunctionCallbackInfo<Value>& args);
void write_uint32(const FunctionCallbackInfo<Value>& args);
void write_uint64(const FunctionCallbackInfo<Value>& args);
void write_double(const FunctionCallbackInfo<Value>& args);
void write_bool(const FunctionCallbackInfo<Value>& args);
void write_string(const FunctionCallbackInfo<Value>& args);

void pop_buffer(const FunctionCallbackInfo<Value>& args);
void push_buffer(const FunctionCallbackInfo<Value>& args);

#endif /* BLOCK_WRAP_H_ */
