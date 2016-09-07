/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Server_Config.h"
#include "Master_Manager.h"
#include "Master_Timer.h"
#include "Master_Server.h"
#include "Master_Connector.h"
#include "Log_Connector.h"
#include "Msg_Manager.h"

Master_Manager::Master_Manager(void):
	player_gate_cid_map_(get_hash_table_size(12000)),
	player_game_cid_map_(get_hash_table_size(12000)),
	player_role_id_map_(get_hash_table_size(12000)),
  server_status_(STATUS_NORMAL),
  msg_count_onoff_(true) { }

Master_Manager::~Master_Manager(void) { }

Master_Manager *Master_Manager::instance_;

Master_Manager *Master_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Master_Manager;
	return instance_;
}

int Master_Manager::init(void) {
	MASTER_TIMER->thr_create();			///	定时器
	MSG_MANAGER->init();
	return 0;
}

void Master_Manager::run_handler(void) {
	process_list();
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

int Master_Manager::self_close_process(void) {
	server_status_ = STATUS_CLOSING;

	//关闭客户端连接
	for (Master_Player_Gate_Cid_Map::iterator iter = player_gate_cid_map_.begin(); iter != player_gate_cid_map_.end(); ++iter) {
		iter->second->link_close();
	}

	int i = 0;
	while (++i < 60) {
		sleep(1);
		LOG_INFO("master server has user:%d", player_gate_cid_map_.size());
		if (player_gate_cid_map_.size() == 0)
			break;
	}

	return 0;
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

int Master_Manager::bind_gate_cid_master_player(int cid, Master_Player &player) {
	player_gate_cid_map_.insert(std::make_pair(cid, &player));
	return 0;
}

int Master_Manager::unbind_gate_cid_master_player(int cid) {
	player_gate_cid_map_.erase(cid);
	return 0;
}

Master_Player* Master_Manager::find_gate_cid_master_player(int cid) {
	Master_Player_Gate_Cid_Map::iterator it = player_gate_cid_map_.find(cid);
	if (it != player_gate_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Master_Manager::bind_game_cid_master_player(int cid, Master_Player &player) {
	player_game_cid_map_.insert(std::make_pair(cid, &player));
	return 0;
}

int Master_Manager::unbind_game_cid_master_player(int cid) {
	player_game_cid_map_.erase(cid);
	return 0;
}

Master_Player* Master_Manager::find_game_cid_master_player(int cid) {
	Master_Player_Game_Cid_Map::iterator it = player_game_cid_map_.find(cid);
	if (it != player_game_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Master_Manager::bind_role_id_master_player(int64_t role_id, Master_Player &player) {
	player_role_id_map_.insert(std::make_pair(role_id, &player));
	return 0;
}

int Master_Manager::unbind_role_id_master_player(int64_t role_id) {
	player_role_id_map_.erase(role_id);
	return 0;
}

Master_Player *Master_Manager::find_role_id_master_player(int64_t role_id) {
	Master_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

int Master_Manager::unbind_master_player(Master_Player &player) {
	player_gate_cid_map_.erase(GET_CID(player.gate_cid(), player.player_cid()));
	player_game_cid_map_.erase(GET_CID(player.game_cid(), player.player_cid()));
	player_role_id_map_.erase(player.role_id());
	return 0;
}

int Master_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	player_tick(now);
	server_info_tick(now);
	object_pool_tick(now);
	//LOG->show_msg_time(now);
	return 0;
}

int Master_Manager::server_info_tick(Time_Value &now) {
	if (now - tick_info_.server_info_last_tick < tick_info_.server_info_interval_tick)
		return 0;

	tick_info_.server_info_last_tick = now;

	master_gate_server_info_.reset();
	master_game_server_info_.reset();
	MASTER_GATE_SERVER->get_server_info(master_gate_server_info_);
	MASTER_GAME_SERVER->get_server_info(master_game_server_info_);

	return 0;
}

int Master_Manager::player_tick(Time_Value &now) {
	if (now - tick_info_.player_last_tick < tick_info_.player_interval_tick)
		return 0;
	tick_info_.player_last_tick = now;
	Master_Player_Role_Id_Map t_role_map(player_role_id_map_); /// 因为Master_Player::time_up()里有改变player_role_id_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	for (Master_Player_Role_Id_Map::iterator it = t_role_map.begin(); it != t_role_map.end(); ++it) {
		if (it->second)
			it->second->tick(now);
	}
	return 0;
}

void Master_Manager::object_pool_tick(Time_Value &now) {
	if (now - tick_info_.object_pool_last_tick < tick_info_.object_pool_interval_tick)
		return;
	tick_info_.object_pool_last_tick = now;
	object_pool_size();
}

void Master_Manager::get_server_info(Block_Buffer &buf) {
	master_gate_server_info_.serialize(buf);
}

void Master_Manager::object_pool_size(void) {
	LOG_INFO("master block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
	LOG_INFO("master player_pool_ free = %d, used = %d", master_player_pool_.free_obj_list_size(), master_player_pool_.used_obj_list_size());
}

void Master_Manager::free_cache(void) {
	Log::instance()->free_cache();
	MASTER_GATE_SERVER->free_cache();
	MASTER_GAME_SERVER->free_cache();

	block_pool_.shrink_all();
	master_player_pool_.shrink_all();
}

void Master_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = inner_msg_count_map_.begin(); it != inner_msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}

void Master_Manager::load_master_db_data(){
	Block_Buffer buf;
	buf.make_server_message(SYNC_MASTER_DB_LOAD_DATA, 0);
	buf.finish_message();
	send_to_db(buf);
}
