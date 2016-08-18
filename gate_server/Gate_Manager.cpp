/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Server_Config.h"
#include "Gate_Server.h"
#include "Gate_Connector.h"
#include "Gate_Client_Messager.h"
#include "Gate_Inner_Messager.h"
#include "Gate_Timer.h"
#include "Gate_Manager.h"

Gate_Manager::Gate_Manager(void):
  player_cid_map_(get_hash_table_size(12000)),
  player_account_map_(get_hash_table_size(12000)),
  server_status_(STATUS_NORMAL),
  verify_pack_onoff_(false),
  msg_count_onoff_(true),
  server_id_(0) { }

Gate_Manager::~Gate_Manager(void) { }

Gate_Manager *Gate_Manager::instance_;

Gate_Manager *Gate_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Gate_Manager;
	return instance_;
}

int Gate_Manager::init(int server_id) {
	tick_time_ = Time_Value::gettimeofday();
	server_id_ = server_id;
	GATE_INNER_MESSAGER;
	GATE_CLIENT_MESSAGER;
	GATE_TIMER->thr_create();

	{ /// 包验证开关
		const Json::Value &server_conf = SERVER_CONFIG->server_conf();
		if (server_conf["verify_pack"].asInt()) {
			verify_pack_onoff_ = true;
		}
	}
	return 0;
}

void Gate_Manager::run_handler(void) {
	process_list();
}

int Gate_Manager::send_to_client(int player_cid, Block_Buffer &buf) {
	if (player_cid < 2) {
		LOG_ERROR("player_cid = %d", player_cid);
		return -1;
	}
	return GATE_CLIENT_SERVER->send_block(player_cid, buf);
}

int Gate_Manager::send_to_game(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		LOG_ERROR("game_cid = %d", cid);
		return -1;
	}
	Game_Connector_Map::iterator iter = game_connector_map_.find(cid);
	if(iter != game_connector_map_.end()) {
		iter->second.connector->send_block(cid, buf);
	} else {
		LOG_ERROR("game_connector_map_ can not find game_cid = %d", cid);
	}
	return 0;
}

int Gate_Manager::send_to_login(Block_Buffer &buf) {
	int login_cid = GATE_LOGIN_CONNECTOR->get_cid();
	if (login_cid < 2) {
		LOG_ERROR("login_cid = %d", login_cid);
		return -1;
	}
	return GATE_LOGIN_CONNECTOR->send_block(login_cid, buf);
}

int Gate_Manager::send_to_master(Block_Buffer &buf) {
	int master_cid = GATE_MASTER_CONNECTOR->get_cid();
	if (master_cid < 2) {
		LOG_ERROR("master_cid = %d", master_cid);
		return -1;
	}
	return GATE_MASTER_CONNECTOR->send_block(master_cid, buf);
}

int Gate_Manager::close_client(int player_cid) {
	if (player_cid >= 2) {
		Close_Info info(player_cid, tick_time());
		close_list_.push_back(info);
	} else {
		LOG_ERROR("close_client, player_cid < 2");
	}
	return 0;
}

int Gate_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;

		//掉线玩家列表
		if (! drop_cid_list_.empty()) {
			all_empty = false;
			cid = drop_cid_list_.pop_front();
			process_drop_cid(cid);
		}
		//client-->gate  消息队列
		if ((buf = gate_client_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_CLIENT_MESSAGER->process_block(*buf);
			} else {
				LOG_ERROR("buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_CLIENT_SERVER->push_block(cid, buf);
		}
		//login-->gate  消息队列
		if ((buf = gate_login_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_INNER_MESSAGER->process_login_block(*buf);
			} else {
				LOG_ERROR("buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_LOGIN_CONNECTOR->push_block(cid, buf);
		}
		//game-->gate  消息队列
		if ((buf = gate_game_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_INNER_MESSAGER->process_game_block(*buf);
			} else {
				LOG_ERROR("buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			Game_Connector_Map::iterator iter = game_connector_map_.find(cid);
			if(iter != game_connector_map_.end()) {
				iter->second.connector->push_block(cid, buf);
			} else {
				LOG_ERROR("game_connector_map_ can not find game_cid = %d", cid);
			}
		}
		//master-->gate  消息队列
		if ((buf = gate_master_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_INNER_MESSAGER->process_master_block(*buf);
			} else {
				LOG_ERROR("buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_MASTER_CONNECTOR->push_block(cid, buf);
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

void Gate_Manager::process_drop_cid(int cid) {
	Gate_Player *player = GATE_MANAGER->find_cid_gate_player(cid);
	if (player) {
		player->link_close();
	}
}

int Gate_Manager::self_close_process(void) {
	server_status_ = STATUS_CLOSING;

	Block_Buffer buf;
	buf.make_inner_message(ACTIVE_DISCONNECT, ERROR_DISCONNECT_SELF);
	buf.finish_message();
	for (Gate_Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		GATE_MANAGER->send_to_client(iter->first, buf);
	}
	//等待服务器通知客户端完毕
	sleep(2);

	//关闭客户端连接
	for (Gate_Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		iter->second->link_close();
	}

	int i = 0;
	while (++i < 60) {
		sleep(1);
		LOG_INFO("gate server has user:%d", player_cid_map_.size());
		if (player_cid_map_.size() == 0)
			break;
	}

	return 0;
}

int Gate_Manager::bind_cid_gate_player(int cid, Gate_Player &player) {
	if (! player_cid_map_.insert(std::make_pair(cid, &player)).second) {
		LOG_ERROR("insert cid gate player error, cid:%d, account:%s", cid, player.get_account().c_str());
	}
	return 0;
}

int Gate_Manager::unbind_cid_gate_player(int cid) {
	player_cid_map_.erase(cid);
	return 0;
}

Gate_Player *Gate_Manager::find_cid_gate_player(int cid) {
	Gate_Player_Cid_Map::iterator it = player_cid_map_.find(cid);
	if (it != player_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Gate_Manager::bind_account_gate_player(std::string& account, Gate_Player &player){
	if (! player_account_map_.insert(std::make_pair(account, &player)).second) {
		LOG_ERROR("insert cid gate player error, account:%s", account.c_str());
	}
	return 0;
}

int Gate_Manager::unbind_account_gate_player(std::string& account){
	player_account_map_.erase(account);
	return 0;
}

Gate_Player *Gate_Manager::find_account_gate_player(std::string& account){
	Gate_Player_Account_Map::iterator it = player_account_map_.find(account);
	if (it != player_account_map_.end())
		return it->second;
	else
		return 0;
}

int Gate_Manager::unbind_gate_player(Gate_Player &player) {
	player_cid_map_.erase(player.get_player_cid());
	player_account_map_.erase(player.get_account());
	return 0;
}

int Gate_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	tick_time_ = now;
	close_list_tick(now);
	player_tick(now);
	server_info_tick(now);
	object_pool_tick(now);
	//LOG->show_msg_time(now);
	return 0;
}

int Gate_Manager::close_list_tick(Time_Value &now) {
	Close_Info info;
	while (! close_list_.empty()) {
		info = close_list_.front();
		if (now - info.timestamp > Time_Value(2, 0)) {
			close_list_.pop_front();
			GATE_CLIENT_SERVER->receive().push_drop(info.cid);
		} else {
			break;
		}
	}
	return 0;
}

int Gate_Manager::server_info_tick(Time_Value &now) {
	if (now - tick_info_.server_info_last_tick < tick_info_.server_info_interval_tick)
		return 0;

	tick_info_.server_info_last_tick = now;

	gate_client_server_info_.reset();
	GATE_CLIENT_SERVER->get_server_info(gate_client_server_info_);

	return 0;
}

int Gate_Manager::player_tick(Time_Value &now) {
	if (now - tick_info_.player_last_tick < tick_info_.player_interval_tick)
		return 0;
	tick_info_.player_last_tick = now;
	Gate_Player_Cid_Map role_map(player_cid_map_); /// 因为Game_Player::time_up()里有改变player_cid_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	for (Gate_Player_Cid_Map::iterator it = role_map.begin(); it != role_map.end(); ++it) {
		if (it->second)
			it->second->tick(now);
	}
	return 0;
}

void Gate_Manager::object_pool_tick(Time_Value &now) {
	if (now - tick_info_.object_pool_last_tick < tick_info_.object_pool_interval_tick)
		return;
	tick_info_.object_pool_last_tick = now;
	object_pool_size();
}

void Gate_Manager::get_server_info(Block_Buffer &buf) {
	gate_client_server_info_.serialize(buf);
}

void Gate_Manager::get_server_ip_port(int player_cid, std::string &ip, int &port) {
	Svc* svc = GATE_CLIENT_SERVER->find_svc(player_cid);
	if (!svc) {
		LOG_ERROR("get_server_ip_port error, cid:%d", player_cid);
		return;
	}
	svc->get_local_addr(ip, port);
}

void Gate_Manager::object_pool_size(void) {
	LOG_INFO("gate block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
	LOG_INFO("gate player_pool_ free = %d, used = %d", gate_player_pool_.free_obj_list_size(), gate_player_pool_.used_obj_list_size());
}

void Gate_Manager::free_cache(void) {
	GATE_CLIENT_SERVER->free_cache();
	GATE_LOGIN_CONNECTOR->free_cache();
	GATE_MASTER_CONNECTOR->free_cache();
	for (Game_Connector_Map::iterator iter = game_connector_map_.begin();
			iter != game_connector_map_.end(); ++iter) {
		iter->second.connector->free_cache();
	}

	block_pool_.shrink_all();
	gate_player_pool_.shrink_all();
}

void Gate_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = inner_msg_count_map_.begin(); it != inner_msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}

void Gate_Manager::new_game_connector(int server_id, std::string server_ip, int server_port, Time_Value &send_interval) {
	Gate_Game_Connector *connector = new Gate_Game_Connector();
	connector->set(server_ip, server_port, send_interval);
	connector->init();
	connector->start();
	int cid = 0;
	if ((cid = connector->connect_server()) < 2) {
		LOG_FATAL("gate_game_connector fatal cid:%d,port:%d", cid, server_port);
	}
	connector->thr_create();
	Gate_Game_Connector_Info connector_info;
	connector_info.connector = connector;
	connector_info.game_cid = cid;
	connector_info.game_server_id = server_id;
	connector_info.game_player_num = 0;
	game_connector_map_.insert(std::make_pair(cid, connector_info));
}

int Gate_Manager::get_game_cid(int game_server_id) {
	for(Game_Connector_Map::iterator iter = game_connector_map_.begin();
			iter != game_connector_map_.end(); iter++) {
		if(iter->second.game_server_id == game_server_id) {
			return iter->second.game_cid;
		}
	}
	return 0;
}

int Gate_Manager::get_lowest_player_game_cid(void) {
	int min_num = 0x7fffffff;
	int game_cid = 0;
	for(Game_Connector_Map::iterator iter = game_connector_map_.begin();
			iter != game_connector_map_.end(); iter++) {
		if(iter->second.game_player_num <= min_num) {
			min_num = iter->second.game_player_num;
			game_cid = iter->second.game_cid;
		}
		iter->second.game_player_num++;
	}
	return game_cid;
}
