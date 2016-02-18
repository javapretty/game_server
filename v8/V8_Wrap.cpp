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
#include "Game_Manager.h"
#include "Game_Client_Messager.h"


Local<Context> create_v8_context(Isolate* isolate) {
	Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
	global->Set(String::NewFromUtf8(isolate, "require", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, require));
	global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, print));
	global->Set(String::NewFromUtf8(isolate, "sleep", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, sleep));
	global->Set(String::NewFromUtf8(isolate, "pop_block", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, pop_block));
	global->Set(String::NewFromUtf8(isolate, "get_player", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player));

	return Context::New(isolate, NULL, global);
}

void require(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return ;
	}

	String::Utf8Value str(args[0]);
	char file_path[128];
	sprintf(file_path, "js/%s", *str);
	run_script(args.GetIsolate(), file_path);
}

void run_script(Isolate* isolate, const char* file_path) {
	HandleScope handle_scope(isolate);
	Local<String> source;
	if (!ReadFile(isolate, file_path).ToLocal(&source)) {
		MSG_USER("script:%s not exist\n", file_path);
		return;
	}
  Local<Script> script = Script::Compile(isolate->GetCurrentContext(), source).ToLocalChecked();
  script->Run(isolate->GetCurrentContext()).ToLocalChecked();
}

void sleep(const FunctionCallbackInfo<Value>& args) {
	Time_Value::sleep(SLEEP_TIME);
}

void print(const FunctionCallbackInfo<Value>& args) {
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

void read_int8(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer *buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		int8_t value = 0;
		value = buf->read_int8();
		args.GetReturnValue().Set(value);
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
		int16_t value = 0;
		value = buf->read_int16();
		args.GetReturnValue().Set(value);
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
		int32_t value = 0;
		value = buf->read_int32();
		args.GetReturnValue().Set(value);
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
		double value = 0;
		value = buf->read_int64();
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
		double value = 0;
		value = buf->read_double();
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
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
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
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
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
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
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
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
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

void write_double(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
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

void write_string(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
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

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "read_double", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), read_double)->GetFunction()) ;

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

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_double", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_double)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "write_string", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), write_string)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "push_block", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), push_block)->GetFunction()) ;

		buf_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "process_login_block", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), process_login_block)->GetFunction()) ;

		args.GetReturnValue().Set(buf_obj);
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_block(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return;
	}

	bool have_data = false;
	if (args.Length() == 2) {
		have_data = true;
	}

	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (buf) {
		if (have_data) {
			GAME_GATE_SERVER->push_block(args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0), buf);
		} else {
			GAME_MANAGER->push_block_buffer(buf);
		}
	}
}

void process_login_block(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
	}

	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	if (!buf) {
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);

	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	switch (msg_id) {
	case REQ_FETCH_ROLE_INFO: {
		MSG_120001 msg;
		if ((ret = msg.deserialize(*buf)) == 0)
			GAME_CLIENT_MESSAGER->process_120001(gate_cid, player_cid, msg);
		break;
	}
	case REQ_CREATE_ROLE: {
		MSG_120002 msg;
		if ((ret = msg.deserialize(*buf)) == 0)
			GAME_CLIENT_MESSAGER->process_120002(gate_cid, player_cid, msg);
		break;
	}
	case SYNC_GATE_GAME_PLAYER_SIGNOUT: {
		ret = GAME_CLIENT_MESSAGER->process_113000(gate_cid, player_cid);
		break;
	}
	default:
		break;
	}
}

void get_player(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
	}

	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(args.GetIsolate());
	localTemplate->SetInternalFieldCount(1);

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Cid_Info cid_info(gate_cid, 0, player_cid);
	Game_Player *player = GAME_MANAGER->find_cid_game_player(cid_info);
	if (player) {
		//将C++指针包装成V8内部对象
		Local<External> player_ptr = External::New(args.GetIsolate(), player);
		// Store the request pointer in the JavaScript wrapper.
		Local<Object> player_obj = localTemplate->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
		player_obj->SetInternalField(0, player_ptr);

		// 为当前对象设置其对外函数接口
		player_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "respond_success_result", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), respond_success_result)->GetFunction()) ;

		player_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "respond_error_result", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), respond_error_result)->GetFunction()) ;

		player_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "role_id", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), role_id)->GetFunction()) ;

		player_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "role_name", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), role_name)->GetFunction()) ;

		args.GetReturnValue().Set(player_obj);
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();

		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_CLIENT_PARAM, player_cid);
		msg_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	}
}

void respond_success_result(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
	}

	Local<Object> obj = args.Holder();
	void* player_ptr = Handle<External>::Cast(obj->GetInternalField(0))->Value();
	Game_Player *player = static_cast<Game_Player*>(player_ptr);
	Block_Buffer *buf = GAME_MANAGER->pop_block_buffer();
	if (!player || !buf) {
		return;
	}

	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	player->respond_success_result(msg_id, buf);
}

void respond_error_result(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
	}

	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Game_Player *player= static_cast<Game_Player*>(raw_obj_ptr);
	if (!player) {
		return;
	}

	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int error_code = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	player->respond_error_result(msg_id, error_code);
}

void role_id(const FunctionCallbackInfo<Value>& args) {
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Game_Player *player= static_cast<Game_Player*>(raw_obj_ptr);
	if (!player) {
		return;
	}
	double role_id = player->game_player_info().role_id;
	args.GetReturnValue().Set(role_id);
}

void role_name(const FunctionCallbackInfo<Value>& args) {
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Game_Player *player= static_cast<Game_Player*>(raw_obj_ptr);
	if (!player) {
		return;
	}
	std::string role_name = player->game_player_info().role_name;
	Local<String> v8Str = String::NewFromUtf8(args.GetIsolate(), role_name.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
	args.GetReturnValue().Set(v8Str);
}
