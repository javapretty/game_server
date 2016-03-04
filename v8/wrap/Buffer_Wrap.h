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

class Block_Buffer;
using namespace v8;

Local<Object> wrap_buffer(Isolate* isolate, Block_Buffer *buf);
Block_Buffer *unwrap_buffer(Local<Object> obj);

//函数说明：新建buffer	参数：无	返回值：buffer对象
void pop_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收buffer	参数：1,buffer	返回值：无
void push_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：重置buffer	参数：无	返回值：无
void buffer_reset(const FunctionCallbackInfo<Value>& args);
//函数说明：生成服务器内部消息buffer	参数：1,msg_id	返回值：无
void make_inner_message(const FunctionCallbackInfo<Value>& args);
//函数说明：生成玩家消息buffer	参数：1,msg_id 2,status 3, player_cid	返回值：无
void make_player_message(const FunctionCallbackInfo<Value>& args);
//函数说明：完成消息buffer的生成	参数：无	返回值：无
void finish_message(const FunctionCallbackInfo<Value>& args);

//函数说明：从buffer读取数据 	参数：无	返回值：读到的数据
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

//函数说明：将数据写到buffer 	参数：1,数据值	返回值：无
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

#endif /* BLOCK_WRAP_H_ */
