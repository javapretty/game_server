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

////使用全局变量保存数据
typedef Block_List<Thread_Mutex> Data_List;
static Data_List v8_data_list_;

void read_int32(const FunctionCallbackInfo<Value>& args);
void read_uint16(const FunctionCallbackInfo<Value>& args);
void read_uint32(const FunctionCallbackInfo<Value>& args);

void Push_V8_Block(Block_Buffer *buf);
Block_Buffer* Pop_V8_Block();
void Pop_Block(Local<String> property, const PropertyCallbackInfo<Value>& info);
void Push_Block(const FunctionCallbackInfo<Value>& args);
void Sleep(const FunctionCallbackInfo<Value>& args);
void Print(const FunctionCallbackInfo<Value>& args);



#endif /* V8_WRAP_H_ */
