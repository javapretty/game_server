/*
 * Wrap_Block_Buffer.cpp
 *
 *  Created on: 2016年1月27日
 *      Author: zhangyalei
 */

#include "V8_Manager.h"
#include "V8_Class.h"
#include "V8_Property.h"
#include "V8_Module.h"
#include "V8_Base.h"

int V8_Manager::wrap_pointer() {
	Block_Buffer *buf = Pop_V8_Block();
	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate_);
	localTemplate->SetInternalFieldCount(1);

	// Wrap the raw C++ pointer in an External so it can be referenced from within JavaScript.
	Local<External> buf_ptr = External::New(isolate_, buf);
	// Store the request pointer in the JavaScript wrapper.
	Local<Object> buf_obj = localTemplate->NewInstance(isolate_->GetCurrentContext()).ToLocalChecked();
	buf_obj->SetInternalField(0, buf_ptr);

	  // 为当前对象设置其对外函数接口
	//buf_obj->Set(isolate_->GetCurrentContext(), String::NewFromUtf8(isolate_, "read_int32", NewStringType::kNormal).ToLocalChecked(),
	//                    FunctionTemplate::New(isolate_, Block_Buffer::read_int32)) ;

	// Set the options object as a property on the global object.
	isolate_->GetCurrentContext()->Global()->Set(isolate_->GetCurrentContext(), String::NewFromUtf8(isolate_, "Pop_V8_Block", NewStringType::kNormal).ToLocalChecked(),
			buf_obj).FromJust();

	return 0;
}

int V8_Manager::wrap_block(void) {
	Isolate* isolate = context_->isolate();

	class_<Block_Buffer> block_buffer(isolate);
	block_buffer
		.ctor()
		.set("peek_int8", &Block_Buffer::peek_int8)
		.set("peek_int16", &Block_Buffer::peek_int16)
		.set("peek_int32", &Block_Buffer::peek_int32)
		.set("peek_int64", &Block_Buffer::peek_int64)
		.set("peek_uint8", &Block_Buffer::peek_uint8)
		.set("peek_uint16", &Block_Buffer::peek_uint16)
		.set("peek_uint32", &Block_Buffer::peek_uint32)
		.set("peek_uint64", &Block_Buffer::peek_uint64)
		.set("peek_double", &Block_Buffer::peek_double)
		.set("peek_bool", &Block_Buffer::peek_bool)
		.set("peek_string", &Block_Buffer::peek_string)
		.set("read_int8", &Block_Buffer::read_int8)
		.set("read_int16", &Block_Buffer::read_int16)
		.set("read_int32", &Block_Buffer::read_int32)
		.set("read_int64", &Block_Buffer::read_int64)
		.set("read_uint8", &Block_Buffer::read_uint8)
		.set("read_uint16", &Block_Buffer::read_uint16)
		.set("read_uint32", &Block_Buffer::read_uint32)
		.set("read_uint64", &Block_Buffer::read_uint64)
		.set("read_double", &Block_Buffer::read_double)
		.set("read_bool", &Block_Buffer::read_bool)
		.set("read_string", &Block_Buffer::read_string)
		.set("write_int8", &Block_Buffer::write_int8)
		.set("write_int16", &Block_Buffer::write_int16)
		.set("write_int32", &Block_Buffer::write_int32)
		.set("write_int64", &Block_Buffer::write_int64)
		.set("write_uint8", &Block_Buffer::write_uint8)
		.set("write_uint16", &Block_Buffer::write_uint16)
		.set("write_uint32", &Block_Buffer::write_uint32)
		.set("write_uint64", &Block_Buffer::write_uint64)
		.set("write_double", &Block_Buffer::write_double)
		.set("write_bool", &Block_Buffer::write_bool)
		.set("write_string", &Block_Buffer::write_string)
		.set("make_inner_message", &Block_Buffer::make_inner_message)
		.set("make_player_message", &Block_Buffer::make_player_message)
		.set("finish_message", &Block_Buffer::finish_message)
	;

	module server(context_->isolate());
	server.set("Block_Buffer", block_buffer);
	server.set("Print", &Print);
	server.set("Get_Block", property(&Get_Block));
	server.set("Push_Game_Gate_Server_Block", &Push_Game_Gate_Server_Block);
	server.set("Sleep", &Sleep);
	context_->isolate()->GetCurrentContext()->Global()->Set(
	    String::NewFromUtf8(isolate, "server"), server.new_instance());

	return 0;
}

