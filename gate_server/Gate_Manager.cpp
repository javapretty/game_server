/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#include "Gate_Client_Messager.h"
#include "Gate_Inner_Messager.h"
#include "Gate_Server.h"
#include "Gate_Connector.h"
#include "Gate_Manager.h"
#include "Gate_Timer.h"
#include "Common_Func.h"
#include "HotUpdate.h"
#include "Configurator.h"
#include "Gate_Player.h"

Gate_Manager::Gate_Manager(void):
  player_cid_map_(get_hash_table_size(12000)),
  is_register_timer_(false),
  verify_pack_onoff_(1),
  msg_count_onoff_(true) {
	register_timer();
}

Gate_Manager::~Gate_Manager(void) {
	unregister_timer();
}

Gate_Manager *Gate_Manager::instance_;

Gate_Manager *Gate_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Gate_Manager;
	return instance_;
}

int Gate_Manager::init(void) {
	tick_time_ = Time_Value::gettimeofday();

	CONFIG_INSTANCE;
	status_ = STATUS_NORMAL;
	md5_key_ = CONFIG_INSTANCE->server_maintainer()["gate_md5_key"].asString();

	GATE_INNER_MESSAGER;					/// 内部消息处理
	GATE_CLIENT_MESSAGER;					/// 外部消息处理
	GATE_TIMER->thr_create();

	{ /// 包验证开关
		const Json::Value &server_misc = CONFIG_INSTANCE->server_misc();
		if (server_misc["no_verify_pack"].asInt()) {
			set_verify_pack_onoff(0);
		}
	}
	return 0;
}

void Gate_Manager::run_handler(void) {
	process_list();
}

int Gate_Manager::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Gate_Manager::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Gate_Manager::time_up(const Time_Value &now) {
	if (! is_register_timer_)
		return 0;

	return 0;
}

Gate_Player *Gate_Manager::pop_gate_player(void) {
	return gate_player_pool_.pop();
}

int Gate_Manager::push_gate_player(Gate_Player *player) {
	return gate_player_pool_.push(player);
}

int Gate_Manager::send_to_client(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GATE_CLIENT_SERVER->send_block(cid, buf);
}

int Gate_Manager::send_to_game(Block_Buffer &buf) {
	int cid = GATE_GAME_CONNECTOR->get_cid();
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GATE_GAME_CONNECTOR->send_block(cid, buf);
}

int Gate_Manager::send_to_login(Block_Buffer &buf) {
	int cid = GATE_LOGIN_CONNECTOR->get_cid();
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GATE_LOGIN_CONNECTOR->send_block(cid, buf);
}

int Gate_Manager::send_to_master(Block_Buffer &buf) {
	int cid = GATE_MASTER_CONNECTOR->get_cid();
	if (cid < 2) {
		MSG_USER("cid = %d", cid);
		return -1;
	}
	return GATE_MASTER_CONNECTOR->send_block(cid, buf);
}

int Gate_Manager::close_client(int cid) {
	if (cid >= 2) {
		Close_Info info(cid, tick_time());
		close_list_.push_back(info);
	} else {
		MSG_USER_TRACE("cid < 2");
	}
	return 0;
}

int Gate_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;

		/// 掉线玩家列表
		if (! drop_cid_list_.empty()) {
			all_empty = false;
			cid = drop_cid_list_.pop_front();
			process_drop_cid(cid);
		}
		/// client-->gate  消息队列
		if ((buf = gate_client_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_CLIENT_MESSAGER->process_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_CLIENT_SERVER->push_block(cid, buf);
		}
		/// 游戏服内部循环消息队列
		if (! self_loop_block_list_.empty()) {
			all_empty = false;
			buf = self_loop_block_list_.front();
			self_loop_block_list_.pop_front();
			GATE_INNER_MESSAGER->process_self_loop_block(*buf);
			block_pool_.push(buf);
		}
		/// login-->gate  消息队列
		if ((buf = gate_login_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_INNER_MESSAGER->process_login_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_LOGIN_CONNECTOR->push_block(cid, buf);
		}
		/// game-->gate  消息队列
		if ((buf = gate_game_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_INNER_MESSAGER->process_game_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_GAME_CONNECTOR->push_block(cid, buf);
		}
		/// master-->gate  消息队列
		if ((buf = gate_master_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				cid = buf->peek_int32();
				GATE_INNER_MESSAGER->process_master_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GATE_MASTER_CONNECTOR->push_block(cid, buf);
		}

		if (all_empty)
			Time_Value::sleep(SLEEP_TIME);
	}
	return 0;
}

void Gate_Manager::process_drop_cid(int cid) {
	Gate_Player *player = GATE_MANAGER->find_cid_gate_player(cid);
	if (player) {
		player->link_close();
	}
}

int Gate_Manager::server_status(void) {
	return status_;
}

int Gate_Manager::self_close_process(void) {
	status_ = STATUS_CLOSING;

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
	while (++i < 30) {
		sleep(1);
		MSG_DEBUG("has user:%d", player_cid_map_.size());
		if (player_cid_map_.size() == 0)
			break;
	}

	return 0;
}

int Gate_Manager::bind_cid_gate_player(int cid, Gate_Player &player) {
	if (! player_cid_map_.insert(std::make_pair(cid, &player)).second) {
		MSG_USER_TRACE("insert failure");
	}
	return 0;
}

int Gate_Manager::unbind_cid_gate_player(int cid) {
	if (cid < 2) {
		MSG_USER_TRACE("cid = %d", cid);
		return -1;
	}
	if (player_cid_map_.erase(cid) == 0) {
		MSG_USER_TRACE("erase cid = %d return 0", cid);
		return -1;
	}
	return 0;
}

Gate_Player *Gate_Manager::find_cid_gate_player(int cid) {
	Gate_Player_Cid_Map::iterator it = player_cid_map_.find(cid);
	if (it != player_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Gate_Manager::unbind_gate_player(Gate_Player &player) {
	unbind_cid_gate_player(player.get_cid());
	return 0;
}

int Gate_Manager::bind_account_gate_player(std::string& account, Gate_Player &player){
	if (! player_account_map_.insert(std::make_pair(account, &player)).second) {
			MSG_USER_TRACE("insert failure");
	}
	return 0;
}

int Gate_Manager::unbind_account_gate_player(std::string& account){
	if (player_account_map_.erase(account) == 0) {
				MSG_USER_TRACE("erase account = %s return 0", account.c_str());
				return -1;
	}
	return 0;
}

Gate_Player *Gate_Manager::find_account_gate_player(std::string& account){
	Gate_Player_Account_Map::iterator it = player_account_map_.find(account);
	if (it != player_account_map_.end())
		return it->second;
	else
		return 0;
}

int Gate_Manager::tick(void) {
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

int Gate_Manager::manager_tick(Time_Value &now) {
	if (now - tick_info_.manager_last_tick < tick_info_.manager_interval_tick)
		return 0;
	tick_info_.manager_last_tick = now;
	this->time_up(now);
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
		MSG_DEBUG("get_server_ip_port wrong, cid:%d", player_cid);
		return;
	}
	svc->get_local_addr(ip, port);
}

void Gate_Manager::object_pool_size(void) {
	MSG_DEBUG("Gate_Manager Object_Pool Size ==============================================================");
	MSG_DEBUG("block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
	MSG_DEBUG("gate_player_pool_ free = %d, used = %d", gate_player_pool_.free_obj_list_size(), gate_player_pool_.used_obj_list_size());
}

void Gate_Manager::free_cache(void) {
	MSG_DEBUG("REQ_FREE_CACHE");

	GATE_CLIENT_SERVER->free_cache();
	GATE_LOGIN_CONNECTOR->free_cache();
	GATE_GAME_CONNECTOR->free_cache();
	GATE_MASTER_CONNECTOR->free_cache();

	block_pool_.shrink_all();
	gate_player_pool_.shrink_all();
}

void Gate_Manager::set_verify_pack_onoff(int v) {
	verify_pack_onoff_ = v;
}

void Gate_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = inner_msg_count_map_.begin(); it != inner_msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	MSG_USER("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
}
