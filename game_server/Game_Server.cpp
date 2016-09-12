/*
 * Game_Server.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#include "Game_Server.h"
#include "Game_Manager.h"

Game_Gate_Server::Game_Gate_Server(void) { }

Game_Gate_Server::~Game_Gate_Server(void) { }

Game_Gate_Server *Game_Gate_Server::instance_ = 0;

Game_Gate_Server *Game_Gate_Server::instance(void) {
	if (! instance_)
		instance_ = new Game_Gate_Server;
	return instance_;
}

void Game_Gate_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		bool all_empty = true;
		if (!drop_cid_list_.empty()) {
			all_empty = false;
			int cid = drop_cid_list_.pop_front();
			GAME_MANAGER->push_drop_gate_cid(cid);
		}
		if (!block_list_.empty()) {
			all_empty = false;
			buf = block_list_.pop_front();
			GAME_MANAGER->push_game_gate_data(buf);
		}
		if (all_empty) {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
