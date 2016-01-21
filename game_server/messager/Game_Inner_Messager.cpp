/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "Game_Inner_Messager.h"
#include "Game_Manager.h"
#include "Log.h"
#include "Configurator.h"
#include "Game_Player.h"

Game_Inner_Messager::Game_Inner_Messager(void) { }

Game_Inner_Messager::~Game_Inner_Messager(void) { }

Game_Inner_Messager *Game_Inner_Messager::instance_;

Game_Inner_Messager *Game_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Game_Inner_Messager;
	return instance_;
}

int Game_Inner_Messager::process_db_block(Block_Buffer &buf) {
	int32_t cid = 0;
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t status = 0;

	buf.read_int32(cid);
	buf.read_uint16(len);
	buf.read_uint32(msg_id);
	buf.read_int32(status);

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_DB_GAME_LOAD_DB_CACHE: {
		process_load_db_cache(buf);
		break;
	}
	case SYNC_DB_GAME_LOAD_PLAYER_INFO:
	case SYNC_DB_GAME_CREATE_PLAYER: {
		process_loaded_player_data(buf);
		break;
	}
	case SYNC_DB_GAME_SAVE_PLAYER_INFO: {
		process_save_player_complete(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

int Game_Inner_Messager::process_master_block(Block_Buffer &buf) {
	int32_t cid = 0;
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t status = 0;

	buf.read_int32(cid);
	buf.read_uint16(len);
	buf.read_uint32(msg_id);
	buf.read_int32(status);

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	default:
		break;
	}

	return 0;
}

int Game_Inner_Messager::process_self_loop_block(Block_Buffer &buf) {
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t status = 0;

	buf.read_uint16(len);
	buf.read_uint32(msg_id);
	buf.read_int32(status);

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_INNER_TIMER_TICK: {
		GAME_MANAGER->tick();
		break;
	}
	case SYNC_INNER_CONFIG_HOTUPDATE: {
		process_400001(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

int Game_Inner_Messager::process_load_db_cache(Block_Buffer &buf) {
	MSG_550000 msg;
	msg.deserialize(buf);
	for (std::vector<Player_DB_Cache>::iterator iter = msg.db_cache_vec.begin(); iter != msg.db_cache_vec.end(); ++iter) {
		GAME_MANAGER->db_cache()->id_player_cache_map.insert(std::make_pair(iter->role_id, *iter));
		GAME_MANAGER->db_cache()->account_player_cache_map.insert(std::make_pair(iter->account, *iter));
	}
	MSG_DEBUG("load_db_cache success, role count:%d", msg.db_cache_vec.size());
	return 0;
}
int Game_Inner_Messager::process_loaded_player_data(Block_Buffer &buf) {
	MSG_550001 msg;
	msg.deserialize(buf);
	Game_Manager::Logining_Map::iterator logining_it = GAME_MANAGER->logining_map().find(msg.player_data.game_player_info.account);
	if (logining_it == GAME_MANAGER->logining_map().end()) {
		MSG_USER("account not exist in logining map, account = %s.", msg.player_data.game_player_info.account.c_str());
		return -1;
	}

	int gate_cid = logining_it->second.gate_cid;
	int player_cid = logining_it->second.player_cid;
	if (gate_cid < 2 || player_cid < 2) {
		MSG_USER("gate_cid = %d, player_cid = %d", gate_cid, player_cid);
		return -1;
	}

	GAME_MANAGER->logining_map().erase(logining_it);

	switch(msg.player_data.status) {
	case Player_Data::ROLE_NOT_EXIST:	{
		/// [登录]没有此玩家数据, 提示创建新角色
		Block_Buffer res_buf;
		res_buf.make_message(RES_FETCH_ROLE_INFO, ERROR_ROLE_NOT_EXIST, player_cid);
		MSG_520001 res_msg;
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, res_buf);
		break;
	}
	case Player_Data::ROLE_HAS_EXIST: {
		/// [创建角色]创建的角色名已存在
		Block_Buffer res_buf;
		res_buf.make_message(RES_CREATE_ROLE, ERROR_EXIST_ROLE_NAME, player_cid);
		MSG_520002 res_msg;
		res_msg.role_id = 0;
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, res_buf);
		break;
	}
	case Player_Data::SUCCESS_CREATED: {
		/// [创建角色]创建角色成功
		Block_Buffer res_buf;
		res_buf.make_message(RES_CREATE_ROLE, 0, player_cid);
		MSG_520002 res_msg;
		res_msg.role_id = msg.player_data.role_id;
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, res_buf);

		process_success_login(gate_cid, player_cid, msg.player_data);
		break;
	}
	case Player_Data::SUCCESS_LOADED: {
		/// 数据加载成功
		process_success_login(gate_cid, player_cid, msg.player_data);
		break;
	}
	default: {
		MSG_USER("unknow status = %d.", msg.player_data.status);
	}
	}

	return 0;
}

int Game_Inner_Messager::process_success_login(int gate_cid, int player_cid, Player_Data &data) {
	Game_Player *player = GAME_MANAGER->pop_game_player();
	if (! player) {
		MSG_USER("game_player_pool_.pop() return 0.");
		return -1;
	}

	player->reset();
	Cid_Info cid_info(gate_cid, 0, player_cid);
	player->set_cid_info(cid_info);
	player->load_player_data(data);
	GAME_MANAGER->bind_gate_cid_game_player(gate_cid, *player);
	GAME_MANAGER->bind_role_id_game_player(player->game_player_info().role_id, *player);
	player->sign_in(data.game_player_info.account);

	Player_DB_Cache db_cache;
	db_cache.role_id = data.game_player_info.role_id;
	db_cache.account = data.game_player_info.account;
	db_cache.role_name = data.game_player_info.role_name;
	db_cache.agent_num = data.game_player_info.agent_num;
	db_cache.server_num = data.game_player_info.server_num;
	db_cache.gender = data.game_player_info.gender;
	db_cache.career = data.game_player_info.career;
	db_cache.level = data.game_player_info.level;
	GAME_MANAGER->db_cache()->id_player_cache_map.insert(std::make_pair(db_cache.role_id, db_cache));
	GAME_MANAGER->db_cache()->account_player_cache_map.insert(std::make_pair(db_cache.account, db_cache));

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

int Game_Inner_Messager::process_400001(Block_Buffer &buf) {
	MSG_400001 msg;
	msg.deserialize(buf);
	CONFIG_INSTANCE->hot_update_conf(msg.module, 1);
	return 0;
}
