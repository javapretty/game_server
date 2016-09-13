/*
 * Server_Manager.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Common_Func.h"
#include "Log_Connector.h"
#include "Msg_Manager.h"
#include "Server_Config.h"
#include "Server_Manager.h"

Server_Manager::Server_Manager(void):
  player_cid_map_(get_hash_table_size(12000)),
  player_role_id_map_(get_hash_table_size(12000)),
  player_account_map_(get_hash_table_size(12000)),
  server_id_(0),
	server_status_(STATUS_NORMAL),
	server_start_time_(Time_Value::zero),
	tick_time_(Time_Value::zero),
	player_tick_(Time_Value::zero),
	server_info_tick_(Time_Value::zero),
  msg_count_(false),
  total_recv_bytes(0),
  total_send_bytes(0) { }

Server_Manager::~Server_Manager(void) {}

int Server_Manager::init_data(int server_id, std::string server_name) {
	server_id_ = server_id;
	server_name_ = server_name;
	server_start_time_ = Time_Value::gettimeofday();
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_misc["verify_pack"].asInt()) {
		msg_count_ = true;
	}
	return 0;
}

void Server_Manager::run_handler(void) {
	process_list();
}

int Server_Manager::process_list(void) {
	return 0;
}

int Server_Manager::bind_cid_player(int cid, Player *player) {
	player_cid_map_.insert(std::make_pair(cid, player));
	return 0;
}

Player* Server_Manager::find_cid_player(int cid) {
	Player_Cid_Map::iterator iter = player_cid_map_.find(cid);
	if (iter != player_cid_map_.end())
		return iter->second;
	else
		return 0;
}

int Server_Manager::bind_role_id_player(int64_t role_id, Player *player) {
	player_role_id_map_.insert(std::make_pair(role_id, player));
	return 0;
}

Player* Server_Manager::find_role_id_player(int64_t role_id) {
	Player_RoleId_Map::iterator iter = player_role_id_map_.find(role_id);
	if (iter != player_role_id_map_.end())
		return iter->second;
	else
		return 0;
}

int Server_Manager::bind_account_player(std::string &account, Player *player) {
	player_account_map_.insert(std::make_pair(account, player));
	return 0;
}

Player* Server_Manager::find_account_player(std::string &account) {
	Player_Account_Map::iterator iter = player_account_map_.find(account);
	if (iter != player_account_map_.end())
		return iter->second;
	else
		return 0;
}

int Server_Manager::self_close_process(void) {
	LOG_INFO("%s server_id:%d self close", server_name_.c_str(), server_id_);
	server_status_ = STATUS_CLOSING;

	//关闭客户端连接
	for (Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		close_client(iter->second->gate_cid(), iter->second->player_cid(), ERROR_DISCONNECT_SELF);
	}

	int i = 0;
	while (++i < 60) {
		sleep(1);
		if (player_cid_map_.size() == 0)
			break;
	}

	return 0;
}

int Server_Manager::close_client(int gate_cid, int player_cid, int error_code) {
	if (player_cid < 2) {
		LOG_ERROR("close_client, player_cid = %d", player_cid);
		return -1;
	}

	Close_Info close_info(gate_cid, player_cid, tick_time());
	close_list_.push_back(close_info);
	return 0;
}

int Server_Manager::recycle_player(int gate_cid, int player_cid) {
	LOG_INFO("%s recycle_player, server_id:%d gate_cid:%d, player_cid:%d", server_name_.c_str(), server_id_, gate_cid, player_cid);
	return 0;
}

int Server_Manager::free_cache(void) {
	LOG_CONNECTOR->free_cache();
	block_pool_.shrink_all();
	return 0;
}

int Server_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	tick_time_ = now;
	close_list_tick(now);
	player_tick(now);
	server_info_tick(now);
	return 0;
}

int Server_Manager::close_list_tick(Time_Value &now) {
	Close_Info close_info;
	while (! close_list_.empty()) {
		close_info = close_list_.front();
		if (now - close_info.timestamp > Time_Value(2, 0)) {
			close_list_.pop_front();
			recycle_player(close_info.gate_cid, close_info.player_cid);
		} else {
			break;
		}
	}

	return 0;
}

int Server_Manager::player_tick(Time_Value &now) {
	if (now - player_tick_ < Time_Value(0, 500 * 1000))
		return 0;
	player_tick_ = now;

	for (Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter!= player_cid_map_.end(); ++iter) {
		if (iter->second)
			iter->second->tick(now);
	}
	return 0;
}

int Server_Manager::server_info_tick(Time_Value &now) {
	if (now - server_info_tick_ < Time_Value(300, 0))
		return 0;
	server_info_tick_ = now;

	get_server_info();
	print_server_info();
	print_msg_info();
	return 0;
}

void Server_Manager::get_server_info(void) { }

void Server_Manager::print_server_info(void) {
	LOG_INFO("%s server_id:%d player count:%d", server_name_.c_str(), server_id_, player_cid_map_.size());
	LOG_INFO("%s server_id:%d block_pool_ free = %d, used = %d", server_name_.c_str(), server_id_, block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
}

void Server_Manager::print_msg_info(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = msg_count_map_.begin(); it != msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("%s server_id:%d total_recv_bytes:%d total_send_bytes:%d msg_count:%d %s",
			server_name_.c_str(), server_id_, total_recv_bytes, total_send_bytes, msg_count_map_.size(), stream.str().c_str());
}
