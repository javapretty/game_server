/*
 * V8_Wrap.cpp
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#include "V8_Base.h"
#include "V8_Wrap.h"
#include "Buffer_Wrap.h"
#include "Time_Value.h"
#include "Public_Define.h"
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
	global->Set(String::NewFromUtf8(isolate, "pop_buf", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, pop_buf));
	global->Set(String::NewFromUtf8(isolate, "push_buf", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, push_buf));
	global->Set(String::NewFromUtf8(isolate, "process_login_buf", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, process_login_buf));
	global->Set(String::NewFromUtf8(isolate, "get_player", NewStringType::kNormal).ToLocalChecked(),
			FunctionTemplate::New(isolate, get_player));

	return Context::New(isolate, NULL, global);
}

void require(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		MSG_USER("require args wrong, length: %d\n", args.Length());
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

void process_login_buf(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 4) {
		MSG_USER("process_login_block args wrong, length: %d\n", args.Length());
	}

	Local<Object> buf_obj = args[3]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
	void* buf_ptr = Handle<External>::Cast(buf_obj->GetInternalField(0))->Value();
	Block_Buffer *buf = static_cast<Block_Buffer*>(buf_ptr);
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
		MSG_USER("get_player args wrong, length: %d\n", args.Length());
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
	if (args.Length() != 2) {
		MSG_USER("respond_success_result args wrong, length: %d\n", args.Length());
	}

	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Local<Object> player_obj = args.Holder();
	void* player_ptr = Handle<External>::Cast(player_obj->GetInternalField(0))->Value();
	Game_Player *player = static_cast<Game_Player*>(player_ptr);
	Local<Object> buf_obj = args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
	void* buf_ptr = Handle<External>::Cast(buf_obj->GetInternalField(0))->Value();
	Block_Buffer *buf = static_cast<Block_Buffer*>(buf_ptr);
	if (!player || !buf) {
		return;
	}

	player->respond_success_result(msg_id, buf);
}

void respond_error_result(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		MSG_USER("respond_error_result args wrong, length: %d\n", args.Length());
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
