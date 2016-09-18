/*
 * Server_Wrap.cpp
 *
 *  Created on: Mar 30, 2016
 *      Author: zhangyalei
 */

#include "json/json.h"
#include "V8_Wrap.h"
#include "Entity_Wrap.h"
#include "Server_Wrap.h"
#include "Game_Server.h"
#include "Game_Connector.h"
#include "Game_Manager.h"
#include "Game_Timer.h"
#include "Master_Server.h"
#include "Master_Connector.h"
#include "Master_Manager.h"
#include "Master_Timer.h"
#include "Msg_Manager.h"
#include "Scene_Manager.h"
#include "AI_Manager.h"

void register_game_timer(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("register timer args error, length: %d\n", args.Length());
		return;
	}

	int timer_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int interval = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int first_tick_internal = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	LOG_INFO("register_game_timer,timer_id:%d, interval:%dms, first_tick_internal:%ds\n", timer_id, interval, first_tick_internal);
	GAME_TIMER->register_v8_handler(timer_id, interval, first_tick_internal);
}

void get_game_timer_id(const FunctionCallbackInfo<Value>& args) {
	int timer_id = GAME_TIMER->pop_v8_timer();
	args.GetReturnValue().Set(timer_id);
}

void pop_game_gate_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_gate_data();
	if (buf) {
		GAME_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t gate_cid = 0;
		int16_t len = 0;
		int32_t msg_id = 0;
		int32_t status = 0;
		int32_t player_cid = 0;
		buf->read_int32(gate_cid);
		buf->read_int16(len);
		buf->read_int32(msg_id);
		buf->read_int32(status);
		buf->read_int32(player_cid);

		GAME_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_msg_object(args.GetIsolate(), gate_cid, player_cid, msg_id, status, *buf));
			GAME_GATE_SERVER->push_block(gate_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

void pop_game_master_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_master_data();
	if (buf) {
		GAME_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t master_cid = 0;
		int16_t len = 0;
		int32_t msg_id = 0;
		int32_t status = 0;
		int32_t player_cid = 0;
		buf->read_int32(master_cid);
		buf->read_int16(len);
		buf->read_int32(msg_id);
		buf->read_int32(status);
		buf->read_int32(player_cid);

		GAME_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_msg_object(args.GetIsolate(), master_cid, player_cid, msg_id, status, *buf));
			GAME_MASTER_CONNECTOR->push_block(master_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

void pop_game_db_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = GAME_MANAGER->pop_game_db_data();
	if (buf) {
		GAME_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t db_cid = 0;
		int16_t len = 0;
		int32_t msg_id = 0;
		int32_t status = 0;
		buf->read_int32(db_cid);
		buf->read_int16(len);
		buf->read_int32(msg_id);
		buf->read_int32(status);

		GAME_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_msg_object(args.GetIsolate(), db_cid, 0, msg_id, status, *buf));
			GAME_DB_CONNECTOR->push_block(db_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

void send_game_msg_to_gate(const FunctionCallbackInfo<Value>& args) {
	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int status = args[3]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_player_message(msg_id, status, player_cid);
	if (status == 0) {
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			msg_struct->build_msg_buffer(args.GetIsolate(), args[4]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
		}
	}
	buf.finish_message();
	GAME_MANAGER->send_to_gate(gate_cid, buf);
}

void send_game_msg_to_master(const FunctionCallbackInfo<Value>& args) {
	int player_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int status = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_player_message(msg_id, status, player_cid);
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[3]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
	}
	buf.finish_message();
	GAME_MANAGER->send_to_master(buf);
}

void send_game_msg_to_db(const FunctionCallbackInfo<Value>& args) {
	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_server_message(msg_id, 0);
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
	}
	buf.finish_message();
	GAME_MANAGER->send_to_db(buf);
}

void send_game_msg_to_log(const FunctionCallbackInfo<Value>& args) {
	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_server_message(msg_id, 0);
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
	}
	buf.finish_message();
	GAME_MANAGER->send_to_log(buf);
}

void get_drop_game_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = GAME_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void set_game_player_gate_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("set_game_player_gate_cid args error, length: %d\n", args.Length());
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int cid = GET_CID(gate_cid ,player_cid);
	Game_Player *player = dynamic_cast<Game_Player*>(GAME_MANAGER->find_cid_player(cid));
	if (!player) {
		player = GAME_MANAGER->pop_player();
		GAME_MANAGER->bind_cid_player(cid, player);
	}

	player->set_gate_cid(gate_cid);
	player->set_player_cid(player_cid);
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
	LOG_INFO("register_master_timer,timer_id:%d, interval:%dms, first_tick_internal:%ds\n", timer_id, interval, first_tick_internal);
	MASTER_TIMER->register_v8_handler(timer_id, interval, first_tick_internal);
}

void get_master_timer_id(const FunctionCallbackInfo<Value>& args) {
	int timer_id = MASTER_TIMER->pop_v8_timer();
	args.GetReturnValue().Set(timer_id);
}

void pop_master_gate_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_gate_data();
	if (buf) {
		MASTER_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t gate_cid = 0;
		int16_t len = 0;
		int32_t msg_id = 0;
		int32_t status = 0;
		int32_t player_cid = 0;
		buf->read_int32(gate_cid);
		buf->read_int16(len);
		buf->read_int32(msg_id);
		buf->read_int32(status);
		buf->read_int32(player_cid);

		MASTER_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_msg_object(args.GetIsolate(), gate_cid, player_cid, msg_id, status, *buf));
			MASTER_GATE_SERVER->push_block(gate_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

void pop_master_game_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_game_data();
	if (buf) {
		MASTER_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t game_cid = 0;
		int16_t len = 0;
		int32_t msg_id = 0;
		int32_t status = 0;
		int32_t player_cid = 0;
		buf->read_int32(game_cid);
		buf->read_int16(len);
		buf->read_int32(msg_id);
		buf->read_int32(status);
		buf->read_int32(player_cid);

		MASTER_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_msg_object(args.GetIsolate(), game_cid, player_cid, msg_id, status, *buf));
			MASTER_GAME_SERVER->push_block(game_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

void pop_master_db_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_db_data();
	if (buf) {
		MASTER_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t db_cid = 0;
		int16_t len = 0;
		int32_t msg_id = 0;
		int32_t status = 0;
		buf->read_int32(db_cid);
		buf->read_int16(len);
		buf->read_int32(msg_id);
		buf->read_int32(status);

		MASTER_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_msg_object(args.GetIsolate(), db_cid, 0, msg_id, status, *buf));
			MASTER_DB_CONNECTOR->push_block(db_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

//可以使用curl命令，向服务器发送post消息，格式如下
//curl -d "{\"msg_id\":180000, \"role_name\":\"aa\", \"gold\":1000}" "http://127.0.0.1:8080"
void pop_master_http_msg_object(const FunctionCallbackInfo<Value>& args) {
	Block_Buffer *buf = MASTER_MANAGER->pop_master_http_data();
	if (buf) {
		MASTER_MANAGER->add_recv_bytes(buf->readable_bytes());

		int32_t http_cid = 0;
		std::string post_data = "";
		buf->read_int32(http_cid);
		buf->read_string(post_data);
		LOG_INFO("http post data:%s", post_data.c_str());
		Json::Value value;
	  Json::Reader reader(Json::Features::strictMode());
		if ( !reader.parse(post_data, value) || !value["msg_id"].isInt()) {
			LOG_ERROR("http post data:%s format error or msg_id not int", post_data.c_str());
			args.GetReturnValue().SetNull();
			return;
		}

		int msg_id = value["msg_id"].asInt();
		MASTER_MANAGER->add_msg_count(msg_id);
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			args.GetReturnValue().Set(msg_struct->build_http_msg_object(args.GetIsolate(), http_cid, msg_id, value));
			MASTER_HTTP_SERVER->push_block(http_cid, buf);
		} else {
			args.GetReturnValue().SetNull();
		}
	} else {
		args.GetReturnValue().SetNull();
	}
}

void send_master_msg_to_gate(const FunctionCallbackInfo<Value>& args) {
	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int status = args[3]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_player_message(msg_id, status, player_cid);
	if (status == 0) {
		Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
		if(iter != MSG_MANAGER->msg_struct_id_map().end()){
			Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
			msg_struct->build_msg_buffer(args.GetIsolate(), args[4]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
		}
	}
	buf.finish_message();
	MASTER_MANAGER->send_to_gate(gate_cid, buf);
}

void send_master_msg_to_game(const FunctionCallbackInfo<Value>& args) {
	int game_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int status = args[3]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_player_message(msg_id, status, player_cid);
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[4]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
	}
	buf.finish_message();
	MASTER_MANAGER->send_to_game(game_cid, buf);
}

void send_master_msg_to_db(const FunctionCallbackInfo<Value>& args) {
	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_server_message(msg_id, 0);
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
	}
	buf.finish_message();
	MASTER_MANAGER->send_to_db(buf);
}

void send_master_msg_to_log(const FunctionCallbackInfo<Value>& args) {
	int msg_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Block_Buffer buf;
	buf.make_server_message(msg_id, 0);
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[1]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), buf);
	}
	buf.finish_message();
	MASTER_MANAGER->send_to_log(buf);
}

void send_master_msg_to_http(const FunctionCallbackInfo<Value>& args) {
	int http_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int msg_id = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	std::string str_data = "{\"msg_id\":";
	std::stringstream stream;
	stream << msg_id << ",";
	str_data += stream.str();
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_http_msg_buffer(args.GetIsolate(), args[2]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), str_data);
	}
	str_data = str_data.substr(0, str_data.length()-1);
	stream.str("");
	stream << "}";
	str_data += stream.str();
	LOG_INFO("http response data:%s", str_data.c_str());

	Block_Buffer buf;
	buf.write_string(str_data);
	MASTER_MANAGER->send_to_http(http_cid, buf);
}

void get_drop_master_player_cid(const FunctionCallbackInfo<Value>& args) {
	int cid = MASTER_MANAGER->pop_drop_player_cid();
	args.GetReturnValue().Set(cid);
}

void set_master_player_gate_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("set_master_player_gate_cid args error, length: %d\n", args.Length());
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int64_t role_id = args[2]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Master_Player *player = dynamic_cast<Master_Player*>(MASTER_MANAGER->find_role_id_player(role_id));
	if (!player) {
		player = MASTER_MANAGER->pop_player();
		MASTER_MANAGER->bind_role_id_player(role_id, player);
	}

	player->set_gate_cid(gate_cid);
	player->set_player_cid(player_cid);
	player->set_role_id(role_id);
	MASTER_MANAGER->bind_cid_player(GET_CID(gate_cid ,player_cid), player);
}

void set_master_player_game_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("set_master_player_game_cid args error, length: %d\n", args.Length());
		return;
	}

	int game_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int64_t role_id = args[2]->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	Master_Player *player = dynamic_cast<Master_Player*>(MASTER_MANAGER->find_role_id_player(role_id));
	if (!player) {
		player = MASTER_MANAGER->pop_player();
		MASTER_MANAGER->bind_role_id_player(role_id, player);
	}

	player->set_game_cid(game_cid);
	player->set_player_cid(player_cid);
	player->set_role_id(role_id);
	MASTER_MANAGER->bind_game_cid_player(GET_CID(game_cid ,player_cid), player);
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

void is_scene_in_process(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("is_scene_in_process error, length: %d\n", args.Length());
		return;
	}
	int scene_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	bool is_in = SCENE_MANAGER->has_scene(scene_id);
	args.GetReturnValue().Set(is_in);
}

void thinking_in_ai(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("thinking in ai param error, length: %d\n", args.Length());
		return;
	}
	Isolate *isolate = args.GetIsolate();
	Local<Object> obj = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
	AI_MANAGER->thinking(obj, isolate);
}

void get_scene_entity_by_gate_cid(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("get_scene_entity_by_gate_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}

	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int cid = GET_CID(gate_cid, player_cid);
	Scene_Entity *entity = SCENE_MANAGER->create_scene_entity(cid);
	args.GetReturnValue().Set(wrap_scene_entity(args.GetIsolate(), entity));
}

void get_scene_entity_npc(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 0) {
		LOG_ERROR("get_scene_entity_by_gate_cid args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}
	Scene_Entity *entity = SCENE_MANAGER->create_scene_entity(0);
	args.GetReturnValue().Set(wrap_scene_entity(args.GetIsolate(), entity));
}

void get_cid_function(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 2) {
		LOG_ERROR("get_cid_function args error, length: %d\n", args.Length());
		args.GetReturnValue().SetNull();
		return;
	}
	int gate_cid = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int player_cid = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);

	args.GetReturnValue().Set(GET_CID(gate_cid, player_cid));
}
