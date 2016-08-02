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
#include "Game_Connector.h"
#include "Game_Manager.h"
#include "Game_Timer.h"
#include "Master_Server.h"
#include "Master_Connector.h"
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

void send_game_buffer_to_gate(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("send_game_buffer_to_gate args error, length: %d\n", args.Length());
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf = unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	GAME_MANAGER->send_to_gate(gate_cid, *buf);
}

void send_game_buffer_to_master(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("send_game_buffer_to_master args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	GAME_MANAGER->send_to_master(*buf);
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

void send_game_buffer_to_log(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("send_game_buffer_to_log args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	GAME_MANAGER->send_to_log(*buf);
}

void pop_game_gate_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_gate_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_game_gate_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_game_gate_buffer args error, length: %d\n", args.Length());
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		GAME_GATE_SERVER->push_block(gate_cid, buf);
	}
}

void pop_game_db_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_db_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_game_db_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_game_db_buffer args error, length: %d\n", args.Length());
		return;
	}

	int db_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		GAME_DB_CONNECTOR->push_block(db_cid, buf);
	}
}

void pop_game_master_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_master_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_game_master_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_game_master_buffer args error, length: %d\n", args.Length());
		return;
	}

	int master_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		GAME_MASTER_CONNECTOR->push_block(master_cid, buf);
	}
}

void get_drop_game_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = GAME_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void get_game_player_by_gate_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("get_game_player_by_gate_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int cid = gate_cid * 10000 + player_cid;
	Game_Player *player = GAME_MANAGER->find_cid_game_player(cid);
	if (!player) {
		player = GAME_MANAGER->pop_game_player();
		GAME_MANAGER->bind_cid_game_player(gate_cid * 10000 + player_cid, *player);
	}

	player->set_gate_cid(gate_cid, player_cid);
	args.GetReturnValue().Set(wrap_game_player(args.GetIsolate(), player));
}

void game_close_client(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("game_close_client args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int error_code = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	GAME_MANAGER->close_client(gate_cid, player_cid, error_code);
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

void send_master_buffer_to_gate(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("send_master_buffer_to_gate args error, length: %d\n", args.Length());
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	MASTER_MANAGER->send_to_gate(gate_cid, *buf);
}

void send_master_buffer_to_game(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("send_master_buffer_to_game args error, length: %d\n", args.Length());
		return;
	}

	int game_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	MASTER_MANAGER->send_to_game(game_cid, *buf);
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

void send_master_buffer_to_log(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("send_master_buffer_to_log args error, length: %d\n", args.Length());
		return;
	}

	Block_Buffer *buf = unwrap_buffer(args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (!buf) {
		return;
	}

	MASTER_MANAGER->send_to_log(*buf);
}

void pop_master_gate_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_gate_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_master_gate_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_master_gate_buffer args error, length: %d\n", args.Length());
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		MASTER_GATE_SERVER->push_block(gate_cid, buf);
	}
}

void pop_master_db_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_db_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_master_db_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_master_db_buffer args error, length: %d\n", args.Length());
		return;
	}

	int db_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		MASTER_DB_CONNECTOR->push_block(db_cid, buf);
	}
}

void pop_master_game_buffer(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_game_data();
	if (buf) {
		args.GetReturnValue().Set(wrap_buffer(args.GetIsolate(), buf));
	} else {
		//设置对象为空
		args.GetReturnValue().SetNull();
	}
}

void push_master_game_buffer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("push_master_game_buffer args error, length: %d\n", args.Length());
		return;
	}

	int game_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer *buf= unwrap_buffer(args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
	if (buf) {
		MASTER_GAME_SERVER->push_block(game_cid, buf);
	}
}

void get_drop_master_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = MASTER_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void get_master_player_by_gate_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("get_master_player_by_gate_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int64_t role_id = args[2]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Master_Player *player = MASTER_MANAGER->find_role_id_master_player(role_id);
	if (!player) {
		player = MASTER_MANAGER->pop_master_player();
		MASTER_MANAGER->bind_role_id_master_player(role_id, *player);
	}

	player->set_gate_cid(gate_cid, player_cid);
	player->set_role_id(role_id);
	MASTER_MANAGER->bind_gate_cid_master_player(gate_cid * 10000 + player_cid, *player);
	args.GetReturnValue().Set(wrap_master_player(args.GetIsolate(), player));
}

void get_master_player_by_game_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("get_master_player_by_game_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int game_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int64_t role_id = args[2]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Master_Player *player = MASTER_MANAGER->find_role_id_master_player(role_id);
	if (!player) {
		player = MASTER_MANAGER->pop_master_player();
		MASTER_MANAGER->bind_role_id_master_player(role_id, *player);
	}

	player->set_game_cid(game_cid, player_cid);
	player->set_role_id(role_id);
	MASTER_MANAGER->bind_game_cid_master_player(game_cid * 10000 + player_cid, *player);
	args.GetReturnValue().Set(wrap_master_player(args.GetIsolate(), player));
}

void master_close_client(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("master_close_client args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int error_code = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	MASTER_MANAGER->close_client(gate_cid, player_cid, error_code);
}
