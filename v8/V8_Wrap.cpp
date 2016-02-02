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


Local<Context> Create_V8_Context(Isolate* isolate) {
	Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
	global->Set(String::NewFromUtf8(isolate, "Print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, Print));
	global->Set(String::NewFromUtf8(isolate, "Sleep", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, Sleep));
	global->Set(String::NewFromUtf8(isolate, "Pop_Block", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, Pop_Block));
	global->Set(String::NewFromUtf8(isolate, "Get_Player", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, Get_Player));

	return Context::New(isolate, NULL, global);
}

void Sleep(const FunctionCallbackInfo<Value>& args) {
	Time_Value::sleep(SLEEP_TIME);
}

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

void Read_Int16(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> obj = args.Holder();
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0)) ;
	void* raw_obj_ptr = field->Value() ;
	Block_Buffer * buf= static_cast<Block_Buffer*>(raw_obj_ptr);
	int value = 0;
	if (buf) {
		value = buf->read_int16();
	}
	args.GetReturnValue().Set(value);
}

void Read_Int32(const FunctionCallbackInfo<Value>& args)
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

void Pop_Block(const FunctionCallbackInfo<Value>& args) {
	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(args.GetIsolate());
	localTemplate->SetInternalFieldCount(1);

	Block_Buffer *buf = GAME_MANAGER->pop_game_gate_data();
	if (buf) {
		//将C++指针包装成V8内部对象
		Local<External> external_ptr = External::New(args.GetIsolate(), buf);
		//将指针存在V8对象内部
		Local<Object> external_obj = localTemplate->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
		external_obj->SetInternalField(0, external_ptr);

		// 为当前对象设置其对外函数接口
		external_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "Read_Int16", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), Read_Int16)->GetFunction()) ;

		external_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "Read_Int32", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), Read_Int32)->GetFunction()) ;

		external_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "Push_Block", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), Push_Block)->GetFunction()) ;

		external_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "Process_Login_Block", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), Process_Login_Block)->GetFunction()) ;

		args.GetReturnValue().Set(external_obj);
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void Push_Block(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
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

void Process_Login_Block(const FunctionCallbackInfo<Value>& args) {
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

void Get_Player(const FunctionCallbackInfo<Value>& args) {
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
		Local<External> external_ptr = External::New(args.GetIsolate(), player);
		// Store the request pointer in the JavaScript wrapper.
		Local<Object> external_obj = localTemplate->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
		external_obj->SetInternalField(0, external_ptr);

		// 为当前对象设置其对外函数接口
		external_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "Respond_Success_Result", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), Respond_Success_Result)->GetFunction()) ;

		external_obj->Set(args.GetIsolate()->GetCurrentContext(), String::NewFromUtf8(args.GetIsolate(), "Respond_Error_Result", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(args.GetIsolate(), Respond_Error_Result)->GetFunction()) ;

		args.GetReturnValue().Set(external_obj);
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();

		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_CLIENT_PARAM, player_cid);
		msg_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	}
}

void Respond_Success_Result(const FunctionCallbackInfo<Value>& args) {
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

void Respond_Error_Result(const FunctionCallbackInfo<Value>& args) {
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
