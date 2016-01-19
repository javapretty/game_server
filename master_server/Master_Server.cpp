/*
 * Master_Server.cpp
 *
 *  Created on: 2016年1月13日
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
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			MASTER_MANAGER->push_master_gate_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(SLEEP_TIME);
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
			Time_Value::sleep(SLEEP_TIME);
		}
	}
}
