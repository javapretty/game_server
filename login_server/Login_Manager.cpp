/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Server_Config.h"
#include "Login_Manager.h"
#include "Login_Server.h"
#include "Login_Timer.h"
#include "Login_Client_Messager.h"
#include "Login_Inner_Messager.h"

Login_Manager::Login_Manager(void):
  is_register_timer_(false),
  msg_count_onoff_(true),
  player_cid_map_(get_hash_table_size(12000)) ,
  player_account_map_(get_hash_table_size(12000)) {
	register_timer();
}

Login_Manager::~Login_Manager(void) {
	unregister_timer();
	check_account_.release_mysql_conn();
}

Login_Manager *Login_Manager::instance_;

Login_Manager *Login_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Login_Manager;
	return instance_;
}

int Login_Manager::init(void) {
	tick_time_ = Time_Value::gettimeofday();
	status_ = STATUS_NORMAL;

	SERVER_CONFIG;
	LOGIN_INNER_MESSAGER;					/// 内部消息处理
	LOGIN_CLIENT_MESSAGER;					/// 外部消息处理
	LOGIN_TIMER->thr_create();
	check_account_.connect_mysql_db();
	init_gate_ip();

	return 0;
}

void Login_Manager::run_handler(void) {
	process_list();
}

int Login_Manager::init_gate_ip(void) {
	const Json::Value &server_maintainer = SERVER_CONFIG->server_maintainer();
	if (server_maintainer == Json::Value::null) {
		LOG_FATAL("configure file error.");
		return -1;
	}

	Ip_Info ip_info;
	for (Json::Value::iterator iter = server_maintainer["gate_server_list"].begin();
			iter != server_maintainer["gate_server_list"].end(); ++iter) {
		ip_info.ip = (*iter)["ip"].asString();
		ip_info.port = (*iter)["port"].asInt();
	}
	gate_ip_vec_.push_back(ip_info);

	return 0;
}

void Login_Manager::get_gate_ip(std::string &account, std::string &ip, int &port) {
	long hash = elf_hash(account.c_str(), account.size());
	int index = hash % (gate_ip_vec_.size());
	ip = gate_ip_vec_[index].ip;
	port = gate_ip_vec_[index].port;
}

int Login_Manager::bind_account_login_player(std::string& account, Login_Player *player) {
	if (! player_account_map_.insert(std::make_pair(account, player)).second) {
		LOG_INFO("insert failure");
	}
	return 0;
}

int Login_Manager::unbind_account_login_player(std::string& account) {
	player_account_map_.erase(account);
	return 0;
}

Login_Player *Login_Manager::find_account_login_player(std::string& account) {
	Login_Player_Account_Map::iterator it = player_account_map_.find(account);
	if (it != player_account_map_.end())
		return it->second;
	else
		return 0;
}

int Login_Manager::bind_cid_login_player(int cid, Login_Player *player) {
	if (! player_cid_map_.insert(std::make_pair(cid, player)).second) {
		LOG_TRACE("insert failure");
	}
	return 0;
}

int Login_Manager::unbind_cid_login_player(int cid) {
	player_cid_map_.erase(cid);
	return 0;
}

Login_Player *Login_Manager::find_cid_login_player(int cid) {
	Login_Player_Map::iterator it = player_cid_map_.find(cid);
	if (it != player_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Login_Manager::unbind_login_player(Login_Player &player) {
	player_account_map_.erase(player.login_player_info().account);
	player_cid_map_.erase(player.get_cid());
	return 0;
}

int Login_Manager::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Login_Manager::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Login_Manager::send_to_client(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		LOG_INFO("cid = %d", cid);
		return -1;
	}
	return LOGIN_CLIENT_SERVER->send_block(cid, buf);
}

int Login_Manager::send_to_gate(int cid, Block_Buffer &buf){
	if (cid < 2) {
			LOG_INFO("cid = %d", cid);
			return -1;
	}

	return LOGIN_GATE_SERVER->send_block(cid, buf);
}

int Login_Manager::close_client(int cid) {
	if (cid >= 2) {
		Close_Info info(cid, tick_time());
		close_list_.push_back(info);
	} else {
		LOG_TRACE("cid < 2");
	}
	return 0;
}

int Login_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;

		/// client-->login
		if ((buf = login_client_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				LOGIN_CLIENT_MESSAGER->process_block(*buf);
			} else {
				LOG_INFO("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			LOGIN_CLIENT_SERVER->push_block(cid, buf);
		}
		/// gate-->login
		if ((buf = login_gate_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				LOGIN_INNER_MESSAGER->process_gate_block(*buf);
			} else {
				LOG_INFO("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			LOGIN_GATE_SERVER->push_block(cid, buf);
		}
		/// 掉线玩家列表
		if (! drop_cid_list_.empty()) {
			all_empty = false;
			cid = drop_cid_list_.pop_front();
			process_drop_cid(cid);
		}
		/// 游戏服内部循环消息队列
		if (! self_loop_block_list_.empty()) {
			all_empty = false;
			buf = self_loop_block_list_.front();
			self_loop_block_list_.pop_front();
			LOGIN_INNER_MESSAGER->process_self_loop_block(*buf);
			block_pool_.push(buf);
		}

		if (all_empty)
			Time_Value::sleep(Time_Value(0,100));
	}
	return 0;
}

void Login_Manager::process_drop_cid(int cid) {
	Login_Player *player = LOGIN_MANAGER->find_cid_login_player(cid);
	if (player) {
		player->link_close();
	}
}

int Login_Manager::server_status(void) {
	return status_;
}

int Login_Manager::self_close_process(void) {
	status_ = STATUS_CLOSING;
	return 0;
}

int Login_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	tick_time_ = now;

	close_list_tick(now);
	player_tick(now);
	manager_tick(now);

	server_info_tick(now);
	object_pool_tick(now);
	//LOG->show_msg_time(now);
	return 0;
}

int Login_Manager::close_list_tick(Time_Value &now) {
	Close_Info info;
	while (! close_list_.empty()) {
		info = close_list_.front();
		if (now - info.timestamp > Time_Value(2, 0)) {
			close_list_.pop_front();
			LOGIN_CLIENT_SERVER->receive().push_drop(info.cid);
		} else {
			break;
		}
	}
	return 0;
}

int Login_Manager::server_info_tick(Time_Value &now) {
	if (now - tick_info_.server_info_last_tick < tick_info_.server_info_interval_tick)
		return 0;

	tick_info_.server_info_last_tick = now;

	login_gate_server_info_.reset();
	login_client_server_info_.reset();
	LOGIN_GATE_SERVER->get_server_info(login_gate_server_info_);
	LOGIN_CLIENT_SERVER->get_server_info(login_client_server_info_);

	return 0;
}

int Login_Manager::player_tick(Time_Value &now) {
	if (now - tick_info_.player_last_tick < tick_info_.player_interval_tick)
		return 0;
	tick_info_.player_last_tick = now;
	Login_Player_Account_Map t_accouont_map(player_account_map_); /// 因为Login_Player::time_up()里有改变player_account_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	for (Login_Player_Account_Map::iterator it = t_accouont_map.begin(); it != t_accouont_map.end(); ++it) {
		if (it->second){
			if (now - it->second->login_player_info().session_tick > Recycle_Tick::session_interval_) {
				LOGIN_CLIENT_SERVER->receive().push_drop(it->second->get_cid());	//断开客户端与login的连接
				it->second->link_close();
			}
			it->second->tick(now);
		}
	}

	return 0;
}

int Login_Manager::manager_tick(Time_Value &now) {
	if (now - tick_info_.manager_last_tick < tick_info_.manager_interval_tick)
		return 0;
	tick_info_.manager_last_tick = now;
	return 0;
}

void Login_Manager::object_pool_tick(Time_Value &now) {
	if (now - tick_info_.object_pool_last_tick < tick_info_.object_pool_interval_tick)
		return;
	tick_info_.object_pool_last_tick = now;
	object_pool_size();
}

void Login_Manager::get_server_info(Block_Buffer &buf) {
	login_client_server_info_.serialize(buf);
	login_gate_server_info_.serialize(buf);
}

void Login_Manager::object_pool_size(void) {
	LOG_DEBUG("Login_Mangager Object_Pool Size ==============================================================");
	LOG_DEBUG("block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
}

void Login_Manager::free_cache(void) {
	LOG_DEBUG("REQ_FREE_CACHE");

	LOGIN_CLIENT_SERVER->free_cache();
	LOGIN_GATE_SERVER->free_cache();

	block_pool_.shrink_all();
}

void Login_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = inner_msg_count_map_.begin(); it != inner_msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}

