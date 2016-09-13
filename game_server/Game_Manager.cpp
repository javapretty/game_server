/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Game_Timer.h"
#include "Game_Manager.h"
#include "Game_Server.h"
#include "Game_Connector.h"
#include "Log_Connector.h"
#include "Msg_Manager.h"
#include "AI_Manager.h"

Game_Manager::Game_Manager(void) {}

Game_Manager::~Game_Manager(void) {}

Game_Manager *Game_Manager::instance_;

Game_Manager *Game_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Game_Manager;
	return instance_;
}

int Game_Manager::init(int server_id) {
	init_data(server_id, "Game_Server");
	AI_MANAGER->init();
	GAME_TIMER->thr_create();
	MSG_MANAGER->init();
	return 0;
}

int Game_Manager::close_client(int gate_cid, int player_cid, int error_code) {
	if (Server_Manager::close_client(gate_cid, player_cid, error_code) < 0) return -1;

	//将玩家cid投放到js层
	push_drop_player_cid(GET_CID(gate_cid, player_cid));

	if (error_code != 0) {
		//gate与game断开连接不通知客户端，其他情况通知
		Block_Buffer buf;
		buf.make_player_message(ACTIVE_DISCONNECT, error_code, player_cid);
		buf.finish_message();
		send_to_gate(gate_cid, buf);
	}
	return 0;
}

int Game_Manager::recycle_player(int gate_cid, int player_cid) {
	Server_Manager::recycle_player(gate_cid, player_cid);

	int cid = GET_CID(gate_cid, player_cid);
	Game_Player *player = dynamic_cast<Game_Player*>(find_cid_player(cid));
	if (!player) {
		return -1;
	}

	player_cid_map_.erase(cid);
	player_role_id_map_.erase(player->role_id());
	player_account_map_.erase(player->account());
	player->reset();
	push_player(player);
	return 0;
}

int Game_Manager::free_cache(void) {
	Server_Manager::free_cache();
	player_pool_.shrink_all();
	GAME_GATE_SERVER->free_cache();
	GAME_DB_CONNECTOR->free_cache();
	GAME_MASTER_CONNECTOR->free_cache();
	return 0;
}

void Game_Manager::get_server_info(void) {
	game_gate_server_info_.reset();
	game_db_connector_info_.reset();
	game_master_connector_info_.reset();
	GAME_GATE_SERVER->get_server_info(game_gate_server_info_);
	GAME_DB_CONNECTOR->get_server_info(game_db_connector_info_);
	GAME_MASTER_CONNECTOR->get_server_info(game_master_connector_info_);
}

void Game_Manager::print_server_info(void) {
	Server_Manager::print_server_info();
	LOG_INFO("Game_Server server_id:%d player_pool_ free = %d, used = %d", server_id(), player_pool_.free_obj_list_size(), player_pool_.used_obj_list_size());
}

int Game_Manager::send_to_gate(int gate_cid, Block_Buffer &buf) {
	if (gate_cid < 2) {
		LOG_ERROR("gate_cid = %d", gate_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return GAME_GATE_SERVER->send_block(gate_cid, buf);
}

int Game_Manager::send_to_master(Block_Buffer &buf) {
	int master_cid = GAME_MASTER_CONNECTOR->get_cid();
	if (master_cid < 2) {
		LOG_ERROR("master_cid = %d", master_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return GAME_MASTER_CONNECTOR->send_block(master_cid, buf);
}

int Game_Manager::send_to_db(Block_Buffer &buf) {
	int db_cid = GAME_DB_CONNECTOR->get_cid();
	if (db_cid < 2) {
		LOG_ERROR("db_cid = %d", db_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return GAME_DB_CONNECTOR->send_block(db_cid, buf);
}

int Game_Manager::send_to_log(Block_Buffer &buf) {
	int log_cid = LOG_CONNECTOR->get_cid();
	if (log_cid < 2) {
		LOG_ERROR("db_cid = %d", log_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return LOG_CONNECTOR->send_block(log_cid, buf);
}

int Game_Manager::process_list(void) {
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

int Game_Manager::process_drop_gate_cid(int gate_cid) {
	for (Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ) {
		if (iter->second->gate_cid() == gate_cid) {
			//gate断开与game的连接，game上所有通过该gate连接进来的玩家数据回收
			close_client(gate_cid, iter->second->player_cid(), 0);
			player_cid_map_.erase(iter++);
		}
	}
	return 0;
}
