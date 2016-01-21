/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Game_Manager.h"
#include "HotUpdate.h"
#include "Configurator.h"
#include "Game_Server.h"
#include "Game_Connector.h"
#include "Game_Client_Messager.h"
#include "Game_Inner_Messager.h"
#include "Game_Player.h"


Game_Manager::Game_Manager(void):
	db_cache_(0),
	logining_map_(get_hash_table_size(512)),
  saving_map_(get_hash_table_size(512)),
  player_gate_cid_map_(get_hash_table_size(12000)),
  player_role_id_map_(get_hash_table_size(12000)),
  is_register_timer_(false),
  msg_count_onoff_(true) {
	register_timer();
}

Game_Manager::~Game_Manager(void) {
	unregister_timer();
}

Game_Manager *Game_Manager::instance_;

Game_Manager *Game_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Game_Manager;
	return instance_;
}

const int32_t Game_Manager::saving_player_time = 20;

int Game_Manager::init(void) {
	tick_time_ = Time_Value::gettimeofday();

	CONFIG_INSTANCE;
	status_ = STATUS_NORMAL;

	GAME_INNER_MESSAGER;					/// 内部消息处理
	GAME_CLIENT_MESSAGER;					/// 外部消息处理
	GAME_TIMER->thr_create();			///	定时器
	HOT_UPDATE->thr_create();			/// 配制动态更新

	if ((db_cache_ = new DB_Cache) == 0) {
		MSG_ABORT("new DB_Cache return 0.");
	}
	return 0;
}

void Game_Manager::run_handler(void) {
	process_list();
}

int Game_Manager::load_db_cache(void) {
	Block_Buffer buf;
	buf.make_message(SYNC_GAME_DB_LOAD_DB_CACHE);
	buf.finish_message();
	send_to_db(buf);
	return 0;
}

Game_Player *Game_Manager::pop_game_player(void) {
	return game_player_pool_.pop();
}

int Game_Manager::push_game_player(Game_Player *player) {
	return game_player_pool_.push(player);
}

int Game_Manager::send_to_gate(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GAME_GATE_SERVER->send_block(cid, buf);
}

int Game_Manager::send_to_master(Block_Buffer &buf) {
	int cid = GAME_MASTER_CONNECTOR->get_cid();
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GAME_MASTER_CONNECTOR->send_block(cid, buf);
}

int Game_Manager::send_to_db(Block_Buffer &buf) {
	int cid = GAME_DB_CONNECTOR->get_cid();
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GAME_DB_CONNECTOR->send_block(cid, buf);
}

int Game_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;

		/// 玩家登录加载数据
		if ((buf = loaded_player_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				GAME_INNER_MESSAGER->process_db_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GAME_DB_CONNECTOR->push_block(cid, buf);
		}
		/// 掉线玩家列表
		if (! drop_gate_cid_list_.empty()) {
			all_empty = false;
			cid = drop_gate_cid_list_.pop_front();
			process_drop_gate_cid(cid);
		}
		/// gate-->game
		if ((buf = game_gate_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				GAME_CLIENT_MESSAGER->process_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GAME_GATE_SERVER->push_block(cid, buf);
		}
		/// db-->game
		if ((buf = game_db_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				GAME_INNER_MESSAGER->process_db_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GAME_DB_CONNECTOR->push_block(cid, buf);
		}
		/// master-->game
		if ((buf = game_master_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				GAME_INNER_MESSAGER->process_master_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GAME_MASTER_CONNECTOR->push_block(cid, buf);
		}
		/// 游戏服内部循环消息队列
		if (! self_loop_block_list_.empty()) {
			all_empty = false;
			buf = self_loop_block_list_.front();
			self_loop_block_list_.pop_front();
			GAME_INNER_MESSAGER->process_self_loop_block(*buf);
			block_pool_.push(buf);
		}

		if (all_empty)
			Time_Value::sleep(SLEEP_TIME);
	}
	return 0;
}

void Game_Manager::process_drop_gate_cid(int gate_cid) {
	Game_Player_Gate_Cid_Map::iterator iter = player_gate_cid_map_.find(gate_cid);
	if (iter != player_gate_cid_map_.end()) {
		for (Player_Set::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			(*it)->link_close();
		}

		player_gate_cid_map_.erase(iter);
	} else {
		MSG_USER("process_drop_cid, gate_cid=%d, role_id=%ld", gate_cid);
	}
}

int Game_Manager::server_status(void) {
	return status_;
}

int Game_Manager::bind_gate_cid_game_player(int gate_cid, Game_Player &player) {
	Game_Player_Gate_Cid_Map::iterator iter = player_gate_cid_map_.find(gate_cid);
	if (iter != player_gate_cid_map_.end()) {
		iter->second.insert(&player);
	} else {
		Player_Set player_set;
		player_set.insert(&player);
		player_gate_cid_map_.insert(std::make_pair(gate_cid, player_set));
	}
	return 0;
}

int Game_Manager::unbind_gate_cid_game_player(int gate_cid, Game_Player &player) {
	Game_Player_Gate_Cid_Map::iterator iter = player_gate_cid_map_.find(gate_cid);
	if (iter != player_gate_cid_map_.end()) {
		iter->second.erase(&player);
	} else {
		MSG_USER("unbind_gatecid_game_player wrong, gate_cid=%d, role_id=%ld", gate_cid, player.game_player_info().role_id);
	}
	return 0;
}

int Game_Manager::bind_role_id_game_player(role_id_t role_id, Game_Player &player) {
	if (! player_role_id_map_.insert(std::make_pair(role_id, &player)).second) {
		MSG_USER("insert failure");
	}
	return 0;
}

int Game_Manager::unbind_role_id_game_player(role_id_t role_id) {
	player_role_id_map_.erase(role_id);
	return 0;
}

Game_Player *Game_Manager::find_role_id_game_player(role_id_t role_id) {
	Game_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

int Game_Manager::unbind_game_player(Game_Player &player) {
	unbind_gate_cid_game_player(player.cid_info().gate_cid, player);
	player_role_id_map_.erase(player.game_player_info().role_id);
	return 0;
}

int Game_Manager::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Game_Manager::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Game_Manager::time_up(const Time_Value &now) {
	if (! is_register_timer_)
		return 0;

	return 0;
}

int Game_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	tick_time_ = now;

	player_tick(now);
	manager_tick(now);

	server_info_tick(now);
	object_pool_tick(now);
	//LOG->show_msg_time(now);

	saving_scanner_tick(now);
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

int Game_Manager::player_tick(Time_Value &now) {
	if (now - tick_info_.player_last_tick < tick_info_.player_interval_tick)
		return 0;
	tick_info_.player_last_tick = now;
	Game_Player_Role_Id_Map t_role_map(player_role_id_map_); /// 因为Game_Player::time_up()里有改变player_role_id_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	for (Game_Player_Role_Id_Map::iterator it = t_role_map.begin(); it != t_role_map.end(); ++it) {
		if (it->second)
			it->second->tick(now);
	}
	return 0;
}

int Game_Manager::manager_tick(Time_Value &now) {
	if (now - tick_info_.manager_last_tick < tick_info_.manager_interval_tick)
		return 0;
	tick_info_.manager_last_tick = now;
	this->time_up(now);
	return 0;
}

int Game_Manager::saving_scanner_tick(Time_Value &now) {
	if (now - tick_info_.saving_scanner_last_tick < tick_info_.saving_scanner_interval_tick)
		return 0;

	tick_info_.saving_scanner_last_tick = now;

	for (Saving_Map::iterator iter = saving_map().begin(); iter != saving_map().end();) {
		if (now.sec() - iter->second.timestamp.sec() > saving_player_time) {
			iter = saving_map().erase(iter);
			continue;
		}
		++iter;
	}

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
	MSG_DEBUG("Game_Manager Object_Pool Size ==============================================================");
	MSG_DEBUG("block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
	MSG_DEBUG("game_player_pool_ free = %d, used = %d", game_player_pool_.free_obj_list_size(), game_player_pool_.used_obj_list_size());
}

void Game_Manager::free_cache(void) {
	MSG_DEBUG("REQ_FREE_CACHE");

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
	MSG_USER("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}
