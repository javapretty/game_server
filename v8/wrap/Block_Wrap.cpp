/*
 * Block_Wrap.cpp
 *
 *  Created on: 2016年2月19日
 *      Author: zhangyalei
 */

#include "V8_Base.h"
#include "Block_Wrap.h"
#include "Game_Server.h"
#include "Game_Manager.h"

void read_int8(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		args.GetReturnValue().Set(buf->read_int8());
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_int16(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		args.GetReturnValue().Set(buf->read_int16());
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_int32(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		args.GetReturnValue().Set(buf->read_int32());
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_int64(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		double value = buf->read_int64();
		args.GetReturnValue().Set(value);
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_uint8(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		args.GetReturnValue().Set(buf->read_uint8());
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_uint16(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		args.GetReturnValue().Set(buf->read_uint16());
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_uint32(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		args.GetReturnValue().Set(buf->read_uint32());
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_uint64(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		double value = buf->read_uint64();
		args.GetReturnValue().Set(value);
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_double(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		double value = buf->read_double();
		args.GetReturnValue().Set(value);
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_bool(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		bool value = buf->read_bool();
		args.GetReturnValue().Set(value);
	} else {
		args.GetReturnValue().SetNull();
	}
}

void read_string(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		std::string str = buf->read_string();
		Local<String> v8Str = String::NewFromUtf8(args.GetIsolate(), str.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
		args.GetReturnValue().Set(v8Str);
	} else {
		args.GetReturnValue().SetNull();
	}
}

void write_int8(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int8 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		int8_t value = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_int8(value);
	}
}

void write_int16(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int16 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		int16_t value = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_int16(value);
	}
}

void write_int32(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int32 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		int32_t value = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_int32(value);
	}
}

void write_int64(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int64 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		int64_t value = args[0]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_int64(value);
	}
}

void write_uint8(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int8 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		uint8_t value = args[0]->Uint32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_uint8(value);
	}
}

void write_uint16(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int16 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		uint16_t value = args[0]->Uint32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_uint16(value);
	}
}

void write_uint32(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int32 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		uint32_t value = args[0]->Uint32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_uint32(value);
	}
}

void write_uint64(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_int64 args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		uint64_t value = args[0]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_uint64(value);
	}
}

void write_double(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_double args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		double value = args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_double(value);
	}
}

void write_bool(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_double args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		bool value = args[0]->BooleanValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		buf->write_bool(value);
	}
}

void write_string(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		MSG_USER("write_string args wrong, length: %d\n", args.Length());
		return;
	}
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		String::Utf8Value str(args[0]);
		const char* cstr = ToCString(str);
		buf->write_string(cstr);
	}
}

void pop_block(const FunctionCallbackInfo<Value>& args) {
	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(args.GetIsolate());
	localTemplate->SetInternalFieldCount(1);

	bool have_data = false;
	if (args.Length() == 1) {
		have_data = true;
	}

	Block_Buffer *buf = nullptr;
	if (have_data) {
		//buf from client
		buf = GAME_MANAGER->pop_game_gate_data();
	} else {
		// new buf for write data
		buf = GAME_MANAGER->pop_block_buffer();
	}
	if (buf) {
		//将C++指针包装成V8内部对象
		Local<External> buf_ptr = External::New(args.GetIsolate(), buf);
		//将指针存在V8对象内部
		Local<Object> buf_obj = localTemplate->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
		buf_obj->SetInternalField(0, buf_ptr);

		// 为当前对象设置其对外函数接口
		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_int8", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_int8)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_int16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_int16)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_int32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_int32)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_int64", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_int64)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_uint8", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_uint8)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_uint16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_uint16)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_uint32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_uint32)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_uint64", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_uint64)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_double", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_double)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_bool", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_bool)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_string", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_string)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_int8", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_int8)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_int16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_int16)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_int32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_int32)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_int64", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_int64)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_uint8", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_uint8)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_uint16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_uint16)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_uint32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_uint32)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_uint64", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_uint64)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_double", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_double)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_bool", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_bool)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_string", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_string)->GetFunction()) ;

		args.GetReturnValue().Set(buf_obj);
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_block(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		MSG_USER("process_login_block args wrong, length: %d\n", args.Length());
	}

	Local<Object> buf_obj = args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
	void* buf_ptr = Handle<External>::Cast(buf_obj->GetInternalField(0))->Value();
	Block_Buffer *buf = static_cast<Block_Buffer*>(buf_ptr);
	if (buf) {
		if (args.Length() == 2) {
			GAME_GATE_SERVER->push_block(args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0), buf);
		} else {
			GAME_MANAGER->push_block_buffer(buf);
		}
	}
}
