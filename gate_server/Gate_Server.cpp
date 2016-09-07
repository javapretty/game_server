/*
 * Gate_Server.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#include "Gate_Server.h"
#include "Gate_Manager.h"

Gate_Client_Server::Gate_Client_Server(void) { }

Gate_Client_Server::~Gate_Client_Server(void) { }

Gate_Client_Server *Gate_Client_Server::instance_ = 0;

Gate_Client_Server *Gate_Client_Server::instance(void) {
	if (! instance_)
		instance_ = new Gate_Client_Server;
	return instance_;
}

void Gate_Client_Server::process_list(void) {
	Block_Buffer *buf = 0;
	int cid = 0;
	while (1) {
		bool all_empty = true;
		if (!drop_cid_list_.empty()) {
			all_empty = false;
			cid = drop_cid_list_.pop_front();
			GATE_MANAGER->push_drop_cid(cid);
		}
		if (!block_list_.empty()) {
			all_empty = false;
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_client_data(buf);
		}
		if (all_empty) {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
