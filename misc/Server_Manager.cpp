/*
 * Server_Manager.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Msg_Manager.h"
#include "Server_Manager.h"

Server_Manager::Server_Manager(void):
  player_cid_map_(get_hash_table_size(12000)),
  player_role_id_map_(get_hash_table_size(12000)),
  player_account_map_(get_hash_table_size(12000)),
	server_status_(STATUS_NORMAL),
	last_player_tick_(Time_Value::zero),
	last_server_info_tick_(Time_Value::zero),
	tick_time_(Time_Value::zero),
  msg_count_onoff_(true) { }

Server_Manager::~Server_Manager(void) {}

void Server_Manager::run_handler(void) {
	process_list();
}

int Server_Manager::process_list(void) {
	return 0;
}

int Server_Manager::self_close_process(void) {
	server_status_ = STATUS_CLOSING;

	//关闭客户端连接
	for (Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		iter->second->link_close(true);
	}

	int i = 0;
	while (++i < 60) {
		sleep(1);
		LOG_INFO("%s player count:%d", server_name_.c_str(), player_cid_map_.size());
		if (player_cid_map_.size() == 0)
			break;
	}

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

int Server_Manager::unbind_player(Player &player) {
	player_role_id_map_.erase(player.role_id());
	player_account_map_.erase(player.account());
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
	return 0;
}

int Server_Manager::player_tick(Time_Value &now) {
	if (now - last_player_tick_ < Time_Value(0, 500 * 1000))
		return 0;
	last_player_tick_ = now;

	//因为Player::tick()里有改变player_cid_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	Player_Cid_Map t_player_map(player_cid_map_);
	for (Player_Cid_Map::iterator iter = t_player_map.begin(); iter!= t_player_map.end(); ++iter) {
		if (iter->second)
			iter->second->tick(now);
	}
	return 0;
}

int Server_Manager::server_info_tick(Time_Value &now) {
	if (now - last_server_info_tick_ < Time_Value(300, 0))
		return 0;
	last_server_info_tick_ = now;

	get_server_info();
	print_object_pool();
	print_msg_count();
	return 0;
}

void Server_Manager::get_server_info(void) { }

void Server_Manager::free_cache(void) {
	block_pool_.shrink_all();
}

void Server_Manager::print_object_pool(void) {
	LOG_INFO("%s block_pool_ free = %d, used = %d", server_name_.c_str(), block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
}

void Server_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = msg_count_map_.begin(); it != msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("%s msg count=%d content=%s", server_name_.c_str(), msg_count_map_.size(), stream.str().c_str());
}
