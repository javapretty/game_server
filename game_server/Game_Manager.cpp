/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Game_Timer.h"
#include "Game_Manager.h"
#include "Game_Server.h"
#include "Game_Connector.h"
#include "Log_Connector.h"
#include "Msg_Manager.h"
#include "Scene_Manager.h"

Game_Manager::Game_Manager(void):
  player_cid_map_(get_hash_table_size(12000)),
  server_status_(STATUS_NORMAL),
  msg_count_onoff_(true),
  server_id_(0){ }

Game_Manager::~Game_Manager(void) {}

Game_Manager *Game_Manager::instance_;

Game_Manager *Game_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Game_Manager;
	return instance_;
}

int Game_Manager::init(int id) {
	GAME_TIMER->thr_create();			///定时器
	MSG_MANAGER->init();
	SCENE_MANAGER->create_new_scene(1001);
	server_id_ = id;
	return 0;
}

void Game_Manager::run_handler(void) {
	process_list();
}

int Game_Manager::send_to_gate(int gate_cid, Block_Buffer &buf) {
	if (gate_cid < 2) {
		LOG_ERROR("gate_cid = %d", gate_cid);
		return -1;
	}
	return GAME_GATE_SERVER->send_block(gate_cid, buf);
}

int Game_Manager::send_to_master(Block_Buffer &buf) {
	int master_cid = GAME_MASTER_CONNECTOR->get_cid();
	if (master_cid < 2) {
		LOG_ERROR("master_cid = %d", master_cid);
		return -1;
	}
	return GAME_MASTER_CONNECTOR->send_block(master_cid, buf);
}

int Game_Manager::send_to_db(Block_Buffer &buf) {
	int db_cid = GAME_DB_CONNECTOR->get_cid();
	if (db_cid < 2) {
		LOG_ERROR("db_cid = %d", db_cid);
		return -1;
	}
	return GAME_DB_CONNECTOR->send_block(db_cid, buf);
}

int Game_Manager::send_to_log(Block_Buffer &buf) {
	int log_cid = LOG_CONNECTOR->get_cid();
	if (log_cid < 2) {
		LOG_ERROR("db_cid = %d", log_cid);
		return -1;
	}
	return LOG_CONNECTOR->send_block(log_cid, buf);
}

int Game_Manager::close_client(int gate_cid, int player_cid, int error_code) {
	Block_Buffer buf;
	buf.make_player_message(ACTIVE_DISCONNECT, error_code, player_cid);
	buf.finish_message();
	return send_to_gate(gate_cid, buf);
}

int Game_Manager::self_close_process(void) {
	server_status_ = STATUS_CLOSING;

	//关闭客户端连接
	for (Game_Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		iter->second->link_close();
	}

	int i = 0;
	while (++i < 60) {
		sleep(1);
		LOG_INFO("game server has user:%d", player_cid_map_.size());
		if (player_cid_map_.size() == 0)
			break;
	}

	return 0;
}

int Game_Manager::process_list(void) {
	int32_t cid = 0;

	while (1) {
		bool all_empty = true;

		//掉线玩家列表
		if (! drop_gate_cid_list_.empty()) {
			all_empty = false;
			cid = drop_gate_cid_list_.pop_front();
			process_drop_gate_cid(cid);
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

void Game_Manager::process_drop_gate_cid(int gate_cid) {
	for (Game_Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ) {
		if (iter->second->gate_cid() == gate_cid) {
			iter->second->link_close();
			player_cid_map_.erase(iter++);
		}
	}
}

int Game_Manager::bind_cid_game_player(int cid, Game_Player &player) {
	player_cid_map_.insert(std::make_pair(cid, &player));
	return 0;
}

int Game_Manager::unbind_cid_game_player(int cid) {
	player_cid_map_.erase(cid);
	return 0;
}

Game_Player* Game_Manager::find_cid_game_player(int cid) {
	Game_Player_Cid_Map::iterator it = player_cid_map_.find(cid);
	if (it != player_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Game_Manager::unbind_game_player(Game_Player &player) {
	player_cid_map_.erase(player.gate_cid() * 10000 + player.player_cid());
	return 0;
}

int Game_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	player_tick(now);
	server_info_tick(now);
	object_pool_tick(now);
	//LOG->show_msg_time(now);
	SCENE_MANAGER->tick(now);
	return 0;
}

int Game_Manager::player_tick(Time_Value &now) {
	if (now - tick_info_.player_last_tick < tick_info_.player_interval_tick)
		return 0;
	tick_info_.player_last_tick = now;
	Game_Player_Cid_Map t_role_map(player_cid_map_); /// 因为Game_Player::time_up()里有改变player_cid_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	for (Game_Player_Cid_Map::iterator iter = t_role_map.begin(); iter!= t_role_map.end(); ++iter) {
		if (iter->second)
			iter->second->tick(now);
	}
	return 0;
}

int Game_Manager::server_info_tick(Time_Value &now) {
	if (now - tick_info_.server_info_last_tick < tick_info_.server_info_interval_tick)
		return 0;

	tick_info_.server_info_last_tick = now;

	game_gate_server_info_.reset();
	GAME_GATE_SERVER->get_server_info(game_gate_server_info_);

	return 0;
}

void Game_Manager::object_pool_tick(Time_Value &now) {
	if (now - tick_info_.object_pool_last_tick < tick_info_.object_pool_interval_tick)
		return;
	tick_info_.object_pool_last_tick = now;
	object_pool_size();
}

void Game_Manager::get_server_info(Block_Buffer &buf) {
	game_gate_server_info_.serialize(buf);
}

void Game_Manager::object_pool_size(void) {
	LOG_INFO("game block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
	LOG_INFO("game player_pool_ free = %d, used = %d", game_player_pool_.free_obj_list_size(), game_player_pool_.used_obj_list_size());
}

void Game_Manager::free_cache(void) {
	GAME_GATE_SERVER->free_cache();
	GAME_DB_CONNECTOR->free_cache();
	GAME_MASTER_CONNECTOR->free_cache();

	block_pool_.shrink_all();
	game_player_pool_.shrink_all();
}

void Game_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = inner_msg_count_map_.begin(); it != inner_msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}
