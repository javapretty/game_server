/*
 * Gate_Connector.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#include "Gate_Connector.h"
#include "Gate_Manager.h"

Gate_Game_Connector::Gate_Game_Connector(void){ }

Gate_Game_Connector::~Gate_Game_Connector(void) { }

void Gate_Game_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_game_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

///////////////////////////////////////////////////////////////////

Gate_Login_Connector::Gate_Login_Connector(void) { }

Gate_Login_Connector::~Gate_Login_Connector(void) { }

Gate_Login_Connector *Gate_Login_Connector::instance_ = 0;

Gate_Login_Connector *Gate_Login_Connector::instance(void) {
	if (! instance_)
		instance_ = new Gate_Login_Connector;
	return instance_;
}

void Gate_Login_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_login_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

Gate_Master_Connector::Gate_Master_Connector(void) { }

Gate_Master_Connector::~Gate_Master_Connector(void) { }

Gate_Master_Connector *Gate_Master_Connector::instance_ = 0;

Gate_Master_Connector *Gate_Master_Connector::instance(void) {
	if (! instance_)
		instance_ = new Gate_Master_Connector;
	return instance_;
}

void Gate_Master_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_master_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
