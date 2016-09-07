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
	server_id_(0),
  verify_pack_onoff_(false) { }

Gate_Manager::~Gate_Manager(void) { }

Gate_Manager *Gate_Manager::instance_;

Gate_Manager *Gate_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Gate_Manager;
	return instance_;
}

int Gate_Manager::init(int server_id) {
	server_id_ = server_id;
	set_server_name("Gate_Server");
	GATE_INNER_MESSAGER;
	GATE_CLIENT_MESSAGER;
	GATE_TIMER->thr_create();

	const Json::Value &server_conf = SERVER_CONFIG->server_conf();
	if (server_conf["verify_pack"].asInt()) {
		verify_pack_onoff_ = true;
	}
	return 0;
}

int Gate_Manager::unbind_player(Player &player) {
	Server_Manager::unbind_player(player);
	player_cid_map_.erase(player.player_cid());
	return 0;
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
				buf->peek_int32(cid);
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
				buf->peek_int32(cid);
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
				buf->peek_int32(cid);
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
				buf->peek_int32(cid);
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
	Gate_Player *player = dynamic_cast<Gate_Player*>(GATE_MANAGER->find_cid_player(cid));
	if (player) {
		player->link_close();
	}
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

void Gate_Manager::get_server_info(void) {
	gate_client_server_info_.reset();
	gate_login_connector_info_.reset();
	gate_master_connector_info_.reset();
	GATE_CLIENT_SERVER->get_server_info(gate_client_server_info_);
	GATE_LOGIN_CONNECTOR->get_server_info(gate_login_connector_info_);
	GATE_MASTER_CONNECTOR->get_server_info(gate_master_connector_info_);
	for (Game_Connector_Map::iterator iter = game_connector_map_.begin();
			iter != game_connector_map_.end(); ++iter) {
		iter->second.connector_info.reset();
		iter->second.connector->get_server_info(iter->second.connector_info);
	}
}

void Gate_Manager::free_cache(void) {
	Server_Manager::free_cache();
	player_pool_.shrink_all();
	GATE_CLIENT_SERVER->free_cache();
	GATE_LOGIN_CONNECTOR->free_cache();
	GATE_MASTER_CONNECTOR->free_cache();
	for (Game_Connector_Map::iterator iter = game_connector_map_.begin();
			iter != game_connector_map_.end(); ++iter) {
		iter->second.connector->free_cache();
	}
}

void Gate_Manager::print_object_pool(void) {
	Server_Manager::print_object_pool();
	LOG_INFO("Gate_Server player_pool_ free = %d, used = %d", player_pool_.free_obj_list_size(), player_pool_.used_obj_list_size());
}

void Gate_Manager::get_server_ip_port(int player_cid, std::string &ip, int &port) {
	Svc* svc = GATE_CLIENT_SERVER->find_svc(player_cid);
	if (!svc) {
		LOG_ERROR("get_server_ip_port error, cid:%d", player_cid);
		return;
	}
	svc->get_local_addr(ip, port);
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
			iter->second.game_player_num++;
		}
	}
	return game_cid;
}
