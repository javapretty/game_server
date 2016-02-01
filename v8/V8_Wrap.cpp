/*
 * V8_Wrap.cpp
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#include "V8_Base.h"
#include "V8_Wrap.h"
#include "Time_Value.h"
#include "Public_Define.h"
#include "Game_Server.h"

void Push_V8_Block(Block_Buffer *buf) {
	v8_data_list_.push_back(buf);
}

Block_Buffer* Pop_V8_Block() {
	Block_Buffer *buf = v8_data_list_.pop_front();
	if (buf) {
		return buf;
	} else {
		return nullptr;
	}
}

void read_int32(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_int32();
	}
	args.GetReturnValue().Set(value);
}

void read_uint16(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_uint16();
	}
	args.GetReturnValue().Set(value);
}

void read_uint32(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_uint32();
	}
	args.GetReturnValue().Set(value);
}

void Pop_Block(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(info.GetIsolate());
	localTemplate->SetInternalFieldCount(1);

	// Wrap the raw C++ pointer in an External so it can be referenced from within JavaScript.
	Block_Buffer *buf = Pop_V8_Block();
	if (buf) {
		Local<External> buf_ptr = External::New(info.GetIsolate(), buf);
		// Store the request pointer in the JavaScript wrapper.
		Local<Object> buf_obj = localTemplate->NewInstance(info.GetIsolate()->GetCurrentContext()).ToLocalChecked();
		buf_obj->SetInternalField(0, buf_ptr);

		// 为当前对象设置其对外函数接口
		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "read_int32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), read_int32)->GetFunction()) ;

		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "read_uint16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), read_uint16)->GetFunction()) ;

		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "read_uint32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), read_uint32)->GetFunction()) ;

		buf_obj->Set(info.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(info.GetIsolate(), "Push_Block", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(info.GetIsolate(), Push_Block)->GetFunction()) ;

		info.GetReturnValue().Set(buf_obj);
	} else {
		//设置对象为空
		info.GetReturnValue().SetNull();
	}
}

void Push_Block(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return;
	}

	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		GAME_GATE_SERVER->push_block(args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0), buf);
	}
}
void Push_Game_Gate_Server_Block(int cid, Block_Buffer *buf) {
	GAME_GATE_SERVER->push_block(cid, buf);
}

void Sleep(const FunctionCallbackInfo<Value>& args) {
	Time_Value::sleep(SLEEP_TIME);
}

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void Print(const FunctionCallbackInfo<Value>& args) {
  bool first = true;
  for (int i = 0; i < args.Length(); i++) {
    HandleScope handle_scope(args.GetIsolate());
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    printf("%s", cstr);
  }
  printf("\n");
  fflush(stdout);
}


