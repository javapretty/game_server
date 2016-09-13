/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

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

int Master_Manager::init(int server_id) {
	init_data(server_id, "Master_Server");
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

int Master_Manager::close_client(int gate_cid, int player_cid, int error_code) {
	if (Server_Manager::close_client(gate_cid, player_cid, error_code) < 0) return -1;

	//将玩家cid投放到js层
	push_drop_player_cid(GET_CID(gate_cid, player_cid));

	if (error_code != 0) {
		//gate与master断开连接不通知客户端，其他情况通知
		Block_Buffer buf;
		buf.make_player_message(ACTIVE_DISCONNECT, error_code, player_cid);
		buf.finish_message();
		send_to_gate(gate_cid, buf);
	}
	return 0;
}

int Master_Manager::recycle_player(int gate_cid, int player_cid) {
	Server_Manager::recycle_player(gate_cid, player_cid);

	int cid = GET_CID(gate_cid, player_cid);
	Master_Player *player = dynamic_cast<Master_Player*>(find_cid_player(cid));
	if (!player) {
		return -1;
	}

	player_cid_map_.erase(cid);
	player_game_cid_map_.erase(GET_CID(player->game_cid(), player->player_cid()));
	player_role_id_map_.erase(player->role_id());
	player_account_map_.erase(player->account());
	player->reset();
	push_player(player);
	return 0;
}

int Master_Manager::free_cache(void) {
	Server_Manager::free_cache();
	player_pool_.shrink_all();
	MASTER_GATE_SERVER->free_cache();
	MASTER_GAME_SERVER->free_cache();
	MASTER_HTTP_SERVER->free_cache();
	MASTER_DB_CONNECTOR->free_cache();
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

void Master_Manager::print_server_info(void) {
	Server_Manager::print_server_info();
	LOG_INFO("Master_Server player_pool_ free = %d, used = %d", player_pool_.free_obj_list_size(), player_pool_.used_obj_list_size());
}

int Master_Manager::send_to_gate(int gate_cid, Block_Buffer &buf) {
	if (gate_cid < 2) {
		LOG_ERROR("gate_cid = %d", gate_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return MASTER_GATE_SERVER->send_block(gate_cid, buf);
}

int Master_Manager::send_to_game(int game_cid, Block_Buffer &buf) {
	if (game_cid < 2) {
		LOG_ERROR("game_cid = %d", game_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return MASTER_GAME_SERVER->send_block(game_cid, buf);
}

int Master_Manager::send_to_db(Block_Buffer &buf) {
	int db_cid = MASTER_DB_CONNECTOR->get_cid();
	if (db_cid < 2) {
		LOG_ERROR("db_cid = %d", db_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return MASTER_DB_CONNECTOR->send_block(db_cid, buf);
}

int Master_Manager::send_to_log(Block_Buffer &buf) {
	int log_cid = LOG_CONNECTOR->get_cid();
	if (log_cid < 2) {
		LOG_ERROR("log_cid = %d", log_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return LOG_CONNECTOR->send_block(log_cid, buf);
}

int Master_Manager::send_to_http(int http_cid, Block_Buffer &buf) {
	if (http_cid < 2) {
		LOG_ERROR("game_cid = %d", http_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return MASTER_HTTP_SERVER->send_block(http_cid, buf);
}

int Master_Manager::process_list(void) {
	while (1) {
		bool all_empty = true;

		//掉线gate列表
		if (! drop_gate_cid_list_.empty()) {
			all_empty = false;
			int gate_cid = drop_gate_cid_list_.pop_front();
			process_drop_gate_cid(gate_cid);
		}
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

int Master_Manager::process_drop_gate_cid(int gate_cid) {
	for (Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ) {
		if (iter->second->gate_cid() == gate_cid) {
			//gate断开与master的连接，master上所有通过该gate连接进来的玩家数据回收
			close_client(gate_cid, iter->second->player_cid(), 0);
			player_cid_map_.erase(iter++);
		}
	}
	return 0;
}

void Master_Manager::load_master_db_data(){
	Block_Buffer buf;
	buf.make_server_message(SYNC_MASTER_DB_LOAD_DATA, 0);
	buf.finish_message();
	send_to_db(buf);
}
