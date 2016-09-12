/*
 * Master_Server.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#include "Master_Server.h"
#include "Master_Manager.h"

Master_Gate_Server::Master_Gate_Server(void) { }

Master_Gate_Server::~Master_Gate_Server(void) { }

Master_Gate_Server *Master_Gate_Server::instance_ = 0;

Master_Gate_Server *Master_Gate_Server::instance(void) {
	if (! instance_)
		instance_ = new Master_Gate_Server;
	return instance_;
}

void Master_Gate_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		bool all_empty = true;
		if (!drop_cid_list_.empty()) {
			all_empty = false;
			int cid = drop_cid_list_.pop_front();
			MASTER_MANAGER->push_drop_gate_cid(cid);
		}
		if (!block_list_.empty()) {
			all_empty = false;
			buf = block_list_.pop_front();
			MASTER_MANAGER->push_master_gate_data(buf);
		}
		if (all_empty) {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
Master_Game_Server::Master_Game_Server(void) { }

Master_Game_Server::~Master_Game_Server(void) { }

Master_Game_Server *Master_Game_Server::instance_ = 0;

Master_Game_Server *Master_Game_Server::instance(void) {
	if (! instance_)
		instance_ = new Master_Game_Server;
	return instance_;
}

void Master_Game_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			MASTER_MANAGER->push_master_game_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
Master_Http_Server::Master_Http_Server(void) { }

Master_Http_Server::~Master_Http_Server(void) { }

Master_Http_Server *Master_Http_Server::instance_ = 0;

Master_Http_Server *Master_Http_Server::instance(void) {
	if (! instance_)
		instance_ = new Master_Http_Server;
	return instance_;
}

void Master_Http_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			MASTER_MANAGER->push_master_http_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
