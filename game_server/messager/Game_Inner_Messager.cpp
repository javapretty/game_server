/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Game_Inner_Messager.h"
#include "Game_Manager.h"
#include "Server_Config.h"

Game_Inner_Messager::Game_Inner_Messager(void) { }

Game_Inner_Messager::~Game_Inner_Messager(void) { }

Game_Inner_Messager *Game_Inner_Messager::instance_;

Game_Inner_Messager *Game_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Game_Inner_Messager;
	return instance_;
}

int Game_Inner_Messager::process_db_block(Block_Buffer &buf) {
	/*int32_t cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_DB_GAME_LOAD_PLAYER:
	case SYNC_DB_GAME_CREATE_PLAYER: {
		process_loaded_player_data(buf);
		break;
	}
	case SYNC_DB_GAME_SAVE_PLAYER: {
		process_save_player_complete(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

int Game_Inner_Messager::process_master_block(Block_Buffer &buf) {
	/*int32_t cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	default:
		break;
	}

	return 0;
}

int Game_Inner_Messager::process_self_loop_block(Block_Buffer &buf) {
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_INNER_TIMER_TICK: {
		GAME_MANAGER->tick();
		break;
	}
	default:
		break;
	}

	return 0;
}

int Game_Inner_Messager::process_loaded_player_data(Block_Buffer &buf) {
	std::string account = buf.read_string();
	int32_t status = buf.read_int32();
	Game_Manager::Logining_Map::iterator logining_it = GAME_MANAGER->logining_map().find(account);
	if (logining_it == GAME_MANAGER->logining_map().end()) {
		LOG_INFO("account not exist in logining map, account = %s.", account.c_str());
		return -1;
	}

	int gate_cid = logining_it->second.gate_cid;
	int player_cid = logining_it->second.player_cid;
	if (gate_cid < 2 || player_cid < 2) {
		LOG_INFO("player cid error, gate_cid = %d, player_cid = %d, account = %s", gate_cid, player_cid, account.c_str());
		return -1;
	}

	GAME_MANAGER->logining_map().erase(logining_it);

	switch(status) {
	case ROLE_NOT_EXIST:	{
		/// [登录]没有此玩家数据, 提示创建新角色
		Block_Buffer res_buf;
		res_buf.make_player_message(RES_FETCH_ROLE_INFO, ERROR_ROLE_NOT_EXIST, player_cid);
		MSG_520001 res_msg;
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, res_buf);
		break;
	}
	case ROLE_HAS_EXIST: {
		/// [创建角色]创建的角色名已存在
		Block_Buffer res_buf;
		res_buf.make_player_message(RES_CREATE_ROLE, ERROR_ROLE_NAME_EXIST, player_cid);
		MSG_520002 res_msg;
		res_msg.role_id = 0;
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, res_buf);
		break;
	}
	case SUCCESS_CREATED: {
		/// [创建角色]创建角色成功
		Game_Player_Info player_info;
		player_info.deserialize(buf);
		Block_Buffer res_buf;
		res_buf.make_player_message(RES_CREATE_ROLE, 0, player_cid);
		MSG_520002 res_msg;
		res_msg.role_id = player_info.role_id;
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, res_buf);

		process_success_login(gate_cid, player_cid, buf, player_info);
		break;
	}
	case SUCCESS_LOADED: {
		/// 数据加载成功
		Game_Player_Info player_info;
		player_info.deserialize(buf);
		process_success_login(gate_cid, player_cid, buf, player_info);
		break;
	}
	default: {
		LOG_INFO("player_data status:%d error", status);
	}
	}

	return 0;
}

int Game_Inner_Messager::process_success_login(int gate_cid, int player_cid, Block_Buffer &buffer, Game_Player_Info &player_info) {
	Game_Player *player = GAME_MANAGER->pop_game_player();
	if (! player) {
		LOG_ERROR("game_player_pool_ pop error");
		return -1;
	}

	player->reset();
	player->set_cid(gate_cid, player_cid);
	player->load_player(buffer, player_info);
	player->sign_in();

	return 0;
}

int Game_Inner_Messager::process_save_player_complete(Block_Buffer &buf) {
	MSG_550003 msg;
	msg.deserialize(buf);
	if (GAME_MANAGER->saving_map().find(msg.role_id) != GAME_MANAGER->saving_map().end()) {
		GAME_MANAGER->saving_map().erase(msg.role_id);
	}
	return 0;
}
