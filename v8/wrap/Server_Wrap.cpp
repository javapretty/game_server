/*
 * Server_Wrap.cpp
 *
 *  Created on: 2016年3月30日
 *      Author: zhangyalei
 */

#include "V8_Wrap.h"
#include "Buffer_Wrap.h"
#include "Player_Wrap.h"
#include "Server_Wrap.h"
#include "Game_Server.h"
#include "Game_Manager.h"
#include "Game_Timer.h"
#include "Master_Server.h"
#include "Master_Manager.h"
#include "Master_Timer.h"

void register_game_timer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
			LOG_ERROR("register timer args error, length: %d\n", args.Length());
			return;
	}

	int timer_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int interval = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int first_tick_internal = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	LOG_ERROR("register_game_timer,timer_id:%d, interval:%dms, first_tick_internal:%ds\n", timer_id, interval, first_tick_internal);
	GAME_TIMER->register_v8_handler(timer_id, interval, first_tick_internal);
}

void get_game_timer_id(const FunctionCallbackInfo<Value>& args) {
	int timer_id = GAME_TIMER->pop_v8_timer();
	args.GetReturnValue().Set(timer_id);
}

void pop_game_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_block_buffer();
	if (buf) {
		buf->reset();
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_game_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("push_game_buffer args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf= unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		GAME_MANAGER->push_block_buffer(buf);
	}
}

void send_game_buffer_to_db(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("send_game_buffer_to_db args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	GAME_MANAGER->send_to_db(*buf);
}

void send_game_buffer_to_master(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("send_game_buffer_to_db args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	GAME_MANAGER->send_to_master(*buf);
}

void send_master_buffer_to_db(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("send_master_buffer_to_db args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	MASTER_MANAGER->send_to_db(*buf);
}

void pop_game_client_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_client_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_game_client_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_game_client_buffer args error, length: %d\n", args.Length());
		return;
	}

	int cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		GAME_GATE_SERVER->push_block(cid, buf);
	}
}

void get_game_player_load_data_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_player_load_data_buffer();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void get_drop_game_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = GAME_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void get_game_player_by_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("get_game_player_by_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Game_Player *player = GAME_MANAGER->find_cid_game_player(gate_cid * 10000 + player_cid);
	if (player) {
		args.GetReturnValue().Set(wrap_game_player(args.GetIsolate(), player));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
		GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_CLIENT_PARAM);
	}
}

void get_game_player_by_name(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("get_game_player_by_name args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	String::Utf8Value str(args[0]);
	std::string role_name(ToCString(str));
	Game_Player *player = GAME_MANAGER->find_role_name_game_player(role_name);
	if (player) {
		args.GetReturnValue().Set(wrap_game_player(args.GetIsolate(), player));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void pop_sync_master_data_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_sync_master_data_buffer();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
void register_master_timer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
			LOG_ERROR("register_master_timer args error, length: %d\n", args.Length());
			return;
	}

	int timer_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int interval = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int first_tick_internal = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	LOG_ERROR("register_master_timer,timer_id:%d, interval:%dms, first_tick_internal:%ds\n", timer_id, interval, first_tick_internal);
	MASTER_TIMER->register_v8_handler(timer_id, interval, first_tick_internal);
}

void get_master_timer_id(const FunctionCallbackInfo<Value>& args) {
	int timer_id = MASTER_TIMER->pop_v8_timer();
	args.GetReturnValue().Set(timer_id);
}

void pop_master_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_block_buffer();
	if (buf) {
		buf->reset();
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_master_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("push_master_buffer args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf= unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		MASTER_MANAGER->push_block_buffer(buf);
	}
}

void pop_master_client_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_client_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_master_client_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_master_client_buffer args error, length: %d\n", args.Length());
		return;
	}

	int cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		MASTER_GATE_SERVER->push_block(cid, buf);
	}
}

void get_master_player_load_data_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_player_load_data_buffer();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void get_master_db_data_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_public_data_buffer();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void get_drop_master_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = MASTER_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void get_master_player_by_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("get_master_player_by_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Master_Player *player = MASTER_MANAGER->find_gate_cid_master_player(gate_cid * 10000 + player_cid);
	if (player) {
		args.GetReturnValue().Set(wrap_master_player(args.GetIsolate(), player));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
		MASTER_MANAGER->close_client(gate_cid, player_cid, ERROR_CLIENT_PARAM);
	}
}

void get_master_player_by_name(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("get_master_player_by_name args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	String::Utf8Value str(args[0]);
	std::string role_name(ToCString(str));
	Master_Player *player = MASTER_MANAGER->find_role_name_master_player(role_name);
	if (player) {
		args.GetReturnValue().Set(wrap_master_player(args.GetIsolate(), player));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void get_game_player_sync_buffer(const FunctionCallbackInfo<Value>& args){
	Block_Buffer *buf = MASTER_MANAGER->pop_master_player_sync_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void buffer_test(const FunctionCallbackInfo<Value>& args){
	LOG_DEBUG("THIS IS BUFFER_TEST METHOD");
}
