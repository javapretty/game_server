/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "HotUpdate.h"
#include "Configurator.h"
#include "Common_Func.h"
#include "Master_Timer.h"
#include "Master_Player.h"
#include "Master_Manager.h"
#include "Master_Server.h"
#include "Master_Client_Messager.h"
#include "Master_Inner_Messager.h"
#include "Master_Player.h"

Master_Manager::Master_Manager(void):
  player_role_id_map_(get_hash_table_size(12000)),
  is_register_timer_(false),
  msg_count_onoff_(true) {
	register_timer();
}

Master_Manager::~Master_Manager(void) {
	unregister_timer();
}

Master_Manager *Master_Manager::instance_;

Master_Manager *Master_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Master_Manager;
	return instance_;
}

int Master_Manager::init(void) {
	tick_time_ = Time_Value::gettimeofday();

	CONFIG_INSTANCE;
	status_ = STATUS_NORMAL;

	MASTER_INNER_MESSAGER;					/// 内部消息处理
	MASTER_CLIENT_MESSAGER;					/// 外部消息处理
	MASTER_TIMER->thr_create();			///	定时器
	HOT_UPDATE->thr_create();			/// 配制动态更新

	return 0;
}

void Master_Manager::run_handler(void) {
	process_list();
}

int Master_Manager::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Master_Manager::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Master_Manager::time_up(const Time_Value &now) {
	if (! is_register_timer_)
		return 0;

	return 0;
}

Master_Player *Master_Manager::pop_master_player(void) {
	return master_player_pool_.pop();
}

int Master_Manager::push_master_player(Master_Player *player) {
	return master_player_pool_.push(player);
}

int Master_Manager::send_to_gate(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return MASTER_GATE_SERVER->send_block(cid, buf);
}

int Master_Manager::send_to_game(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return MASTER_GAME_SERVER->send_block(cid, buf);
}

int Master_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;
	Player_Data player_data;

	while (1) {
		bool all_empty = true;

		/// gate-->master
		if ((buf = master_gate_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				MASTER_CLIENT_MESSAGER->process_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			MASTER_GATE_SERVER->push_block(cid, buf);
		}
		/// game-->master
		if ((buf = master_game_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				MASTER_INNER_MESSAGER->process_game_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			MASTER_GAME_SERVER->push_block(cid, buf);
		}
		/// 游戏服内部循环消息队列
		if (! self_loop_block_list_.empty()) {
			all_empty = false;
			buf = self_loop_block_list_.front();
			self_loop_block_list_.pop_front();
			MASTER_INNER_MESSAGER->process_self_loop_block(*buf);
			block_pool_.push(buf);
		}

		if (all_empty)
			Time_Value::sleep(SLEEP_TIME);
	}
	return 0;
}

int Master_Manager::server_status(void) {
	return status_;
}

int Master_Manager::bind_role_id_master_player(role_id_t role_id, Master_Player &player) {
	if (! player_role_id_map_.insert(std::make_pair(role_id, &player)).second) {
		MSG_USER_TRACE("insert failure");
	}
	return 0;
}

int Master_Manager::unbind_role_id_master_player(role_id_t role_id) {
	player_role_id_map_.erase(role_id);
	return 0;
}

Master_Player *Master_Manager::find_role_id_master_player(role_id_t role_id) {
	Master_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

int Master_Manager::unbind_master_player(Master_Player &player) {
	player_role_id_map_.erase(player.master_player_info().role_id);
	return 0;
}

int Master_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	tick_time_ = now;

	player_tick(now);
	manager_tick(now);

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

int Master_Manager::manager_tick(Time_Value &now) {
	if (now - tick_info_.manager_last_tick < tick_info_.manager_interval_tick)
		return 0;
	tick_info_.manager_last_tick = now;
	this->time_up(now);
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
	MSG_DEBUG("Master_Manager Object_Pool Size ==============================================================");
	MSG_DEBUG("block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
	MSG_DEBUG("master_player_pool_ free = %d, used = %d", master_player_pool_.free_obj_list_size(), master_player_pool_.used_obj_list_size());
}

void Master_Manager::free_cache(void) {
	MSG_DEBUG("REQ_FREE_CACHE");

	LOG->free_cache();
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
	MSG_USER("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}
