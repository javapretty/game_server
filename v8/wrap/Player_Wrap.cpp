/*
 * Player_Wrap.cpp
 *
 *  Created on: 2016年2月20日
 *      Author: zhangyalei
 */

#include "V8_Base.h"
#include "Buffer_Wrap.h"
#include "Player_Wrap.h"
#include "Block_Buffer.h"
#include "Game_Manager.h"
#include "Game_Client_Messager.h"


Local<Object> wrap_player(Isolate* isolate, Game_Player *player) {
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate);
	localTemplate->SetInternalFieldCount(1);
	Local<External> player_ptr = External::New(isolate, player);
	//将指针存在V8对象内部
	Local<Object> player_obj = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	player_obj->SetInternalField(0, player_ptr);

	// 为当前对象设置其对外函数接口
	player_obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "player_data_buffer", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(isolate, player_data_buffer)->GetFunction()) ;
	player_obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "role_id", NewStringType::kNormal).ToLocalChecked(),
			                    FunctionTemplate::New(isolate, role_id)->GetFunction()) ;
	player_obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "respond_success_result", NewStringType::kNormal).ToLocalChecked(),
	                    FunctionTemplate::New(isolate, respond_success_result)->GetFunction()) ;
	player_obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "respond_error_result", NewStringType::kNormal).ToLocalChecked(),
	                    FunctionTemplate::New(isolate, respond_error_result)->GetFunction()) ;
	player_obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "bag_add_money", NewStringType::kNormal).ToLocalChecked(),
		                    FunctionTemplate::New(isolate, bag_add_money)->GetFunction()) ;
	player_obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "send_mail", NewStringType::kNormal).ToLocalChecked(),
			                    FunctionTemplate::New(isolate, send_mail)->GetFunction()) ;

	return handle_scope.Escape(player_obj);
}

Game_Player *unwrap_player(Local<Object> obj) {
	Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<Game_Player*>(ptr);
}

void process_login_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 4) {
		MSG_USER("process_login_block args wrong, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	int gate_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[3]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);

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

void get_player_data(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_player_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void get_drop_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = GAME_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void get_player_by_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		MSG_USER("get_player args wrong, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Cid_Info cid_info(gate_cid, 0, player_cid);
	Game_Player *player = GAME_MANAGER->find_cid_game_player(cid_info);
	if (player) {
		args.GetReturnValue().Set(wrap_player(args.GetIsolate(), player));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();

		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_CLIENT_PARAM, player_cid);
		msg_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	}
}

void get_player_by_name(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		MSG_USER("get_player args wrong, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	String::Utf8Value str(args[0]);
	std::string role_name(ToCString(str));
	Game_Player *player = GAME_MANAGER->find_role_name_game_player(role_name);
	if (player) {
		args.GetReturnValue().Set(wrap_player(args.GetIsolate(), player));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void player_data_buffer(const FunctionCallbackInfo<Value>& args) {
	Game_Player *player = unwrap_player(args.Holder());
	if (!player) {
		args.GetReturnValue().SetNull();
		return;
	}

	Block_Buffer *buf = player->player_data_buffer();
	if (buf) {
		buf->reset();
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void role_id(const FunctionCallbackInfo<Value>& args) {
	Game_Player *player = unwrap_player(args.Holder());
	if (!player) {
		args.GetReturnValue().Set(0);
	} else {
		double role_id = player->game_player_info().role_id;
		args.GetReturnValue().Set(role_id);
	}
}

void respond_success_result(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		MSG_USER("respond_success_result args wrong, length: %d\n", args.Length());
		return;
	}

	Game_Player *player = unwrap_player(args.Holder());
	if (!player) {
		return;
	}

	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf = unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	player->respond_success_result(msg_id, buf);
}

void respond_error_result(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		MSG_USER("respond_error_result args wrong, length: %d\n", args.Length());
		return;
	}

	Game_Player *player = unwrap_player(args.Holder());
	if (!player) {
		return;
	}

	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int error_code = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	player->respond_error_result(msg_id, error_code);
}

void bag_add_money(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 4) {
		MSG_USER("bag_add_money args wrong, length: %d\n", args.Length());
		return;
	}

	Game_Player *player = unwrap_player(args.Holder());
	if (!player) {
		return;
	}

	int bind_copper = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int copper = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int bind_gold = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int gold = args[4]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);

	std::vector<Money_Add_Info> money_add_list;
	if (bind_copper > 0)
		money_add_list.push_back(Money_Add_Info(BIND_COPPER, bind_copper));
	if (copper > 0)
		money_add_list.push_back(Money_Add_Info(COPPER, copper));
	if (bind_gold > 0)
		money_add_list.push_back(Money_Add_Info(BIND_GOLD, bind_gold));
	if (gold > 0)
		money_add_list.push_back(Money_Add_Info(GOLD, gold));

	if (money_add_list.size() > 0) {
		int result = player->bag().bag_add_money(money_add_list);
		args.GetReturnValue().Set(result);
	}
}

void send_mail(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		MSG_USER("bag_add_money args wrong, length: %d\n", args.Length());
		return;
	}

	Game_Player *player = unwrap_player(args.Holder());
	if (!player) {
		return;
	}

	role_id_t receiver_id = args[0]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Mail_Detail mail_detail;
	player->send_mail(receiver_id, mail_detail);
}
