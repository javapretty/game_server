/*
 * Game_Connector.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#include "Game_Connector.h"
#include "Game_Manager.h"

Game_DB_Connector::Game_DB_Connector(void) { }

Game_DB_Connector::~Game_DB_Connector(void) { }

Game_DB_Connector *Game_DB_Connector::instance_ = 0;

Game_DB_Connector *Game_DB_Connector::instance(void) {
	if (! instance_)
		instance_ = new Game_DB_Connector;
	return instance_;
}

void Game_DB_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GAME_MANAGER->push_game_db_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

Game_Master_Connector::Game_Master_Connector(void) { }

Game_Master_Connector::~Game_Master_Connector(void) { }

Game_Master_Connector *Game_Master_Connector::instance_ = 0;

Game_Master_Connector *Game_Master_Connector::instance(void) {
	if (! instance_)
		instance_ = new Game_Master_Connector;
	return instance_;
}

void Game_Master_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GAME_MANAGER->push_game_master_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
