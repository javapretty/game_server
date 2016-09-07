/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Log_Connector.h"
#include "Server_Config.h"
#include "Master_Manager.h"
#include "Master_Timer.h"
#include "Master_Server.h"
#include "Master_Connector.h"
#include "Msg_Manager.h"

Master_Manager::Master_Manager(void): player_game_cid_map_(get_hash_table_size(12000)) { }

Master_Manager::~Master_Manager(void) { }

Master_Manager *Master_Manager::instance_;

Master_Manager *Master_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Master_Manager;
	return instance_;
}

int Master_Manager::init(void) {
	set_server_name("Master_Server");
	MASTER_TIMER->thr_create();
	MSG_MANAGER->init();
	return 0;
}

int Master_Manager::bind_game_cid_player(int cid, Player *player) {
	player_game_cid_map_.insert(std::make_pair(cid, player));
	return 0;
}

Player* Master_Manager::find_game_cid_player(int cid) {
	Player_Cid_Map::iterator iter = player_game_cid_map_.find(cid);
	if (iter != player_game_cid_map_.end())
		return iter->second;
	else
		return 0;
}

int Master_Manager::unbind_player(Player &player) {
	Server_Manager::unbind_player(player);
	player_cid_map_.erase(GET_CID(player.gate_cid(), player.player_cid()));
	player_game_cid_map_.erase(GET_CID(player.game_cid(), player.player_cid()));
	return 0;
}

int Master_Manager::send_to_gate(int gate_cid, Block_Buffer &buf) {
	if (gate_cid < 2) {
		LOG_ERROR("gate_cid = %d", gate_cid);
		return -1;
	}
	return MASTER_GATE_SERVER->send_block(gate_cid, buf);
}

int Master_Manager::send_to_game(int game_cid, Block_Buffer &buf) {
	if (game_cid < 2) {
		LOG_ERROR("game_cid = %d", game_cid);
		return -1;
	}
	return MASTER_GAME_SERVER->send_block(game_cid, buf);
}

int Master_Manager::send_to_db(Block_Buffer &buf) {
	int db_cid = MASTER_DB_CONNECTOR->get_cid();
	if (db_cid < 2) {
		LOG_ERROR("db_cid = %d", db_cid);
		return -1;
	}
	return MASTER_DB_CONNECTOR->send_block(db_cid, buf);
}

int Master_Manager::send_to_log(Block_Buffer &buf) {
	int log_cid = LOG_CONNECTOR->get_cid();
	if (log_cid < 2) {
		LOG_ERROR("log_cid = %d", log_cid);
		return -1;
	}
	return LOG_CONNECTOR->send_block(log_cid, buf);
}

int Master_Manager::send_to_http(int http_cid, Block_Buffer &buf) {
	if (http_cid < 2) {
		LOG_ERROR("game_cid = %d", http_cid);
		return -1;
	}
	return MASTER_HTTP_SERVER->send_block(http_cid, buf);
}

int Master_Manager::close_client(int gate_cid, int player_cid, int error_code) {
	Block_Buffer buf;
	buf.make_player_message(ACTIVE_DISCONNECT, error_code, player_cid);
	buf.finish_message();
	return send_to_gate(gate_cid, buf);
}

int Master_Manager::process_list(void) {
	while (1) {
		bool all_empty = true;

		//定时器列表
		if (! tick_list_.empty()) {
			all_empty = false;
			tick_list_.pop_front();
			tick();
		}

		if (all_empty)
			Time_Value::sleep(Time_Value(0,100));
	}
	return 0;
}

void Master_Manager::get_server_info(void) {
	master_gate_server_info_.reset();
	master_game_server_info_.reset();
	master_http_server_info_.reset();
	master_db_connector_info_.reset();
	MASTER_GATE_SERVER->get_server_info(master_gate_server_info_);
	MASTER_GAME_SERVER->get_server_info(master_game_server_info_);
	MASTER_HTTP_SERVER->get_server_info(master_http_server_info_);
	MASTER_DB_CONNECTOR->get_server_info(master_db_connector_info_);
}

void Master_Manager::free_cache(void) {
	Server_Manager::free_cache();
	player_pool_.shrink_all();
	MASTER_GATE_SERVER->free_cache();
	MASTER_GAME_SERVER->free_cache();
	MASTER_HTTP_SERVER->free_cache();
	MASTER_DB_CONNECTOR->free_cache();
}

void Master_Manager::print_object_pool(void) {
	Server_Manager::print_object_pool();
	LOG_INFO("Master_Server player_pool_ free = %d, used = %d", player_pool_.free_obj_list_size(), player_pool_.used_obj_list_size());
}

void Master_Manager::load_master_db_data(){
	Block_Buffer buf;
	buf.make_server_message(SYNC_MASTER_DB_LOAD_DATA, 0);
	buf.finish_message();
	send_to_db(buf);
}
