/*
 * Login_Server.cpp
 *
 *  Created on: 2016年1月13日
 *      Author: zhangyalei
 */

#include "Login_Server.h"
#include "Login_Manager.h"

Login_Client_Server::Login_Client_Server(void) { }

Login_Client_Server::~Login_Client_Server(void) { }

Login_Client_Server *Login_Client_Server::instance_ = 0;

Login_Client_Server *Login_Client_Server::instance(void) {
	if (! instance_)
		instance_ = new Login_Client_Server;
	return instance_;
}

void Login_Client_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			LOGIN_MANAGER->push_login_client_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(SLEEP_TIME);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
Login_Gate_Server::Login_Gate_Server(void) { }

Login_Gate_Server::~Login_Gate_Server(void) { }

Login_Gate_Server *Login_Gate_Server::instance_ = 0;

Login_Gate_Server *Login_Gate_Server::instance(void) {
	if (! instance_)
		instance_ = new Login_Gate_Server;
	return instance_;
}

void Login_Gate_Server::process_list(void) {
	Block_Buffer *buf = 0;
	int cid = 0;
	while (1) {
		bool all_empty = true;

		if (!drop_cid_list_.empty()) {
			all_empty = false;
			cid = drop_cid_list_.pop_front();
			LOGIN_MANAGER->push_drop_cid(cid);
		}

		if (!block_list_.empty()) {
			all_empty = false;
			buf = block_list_.pop_front();
			LOGIN_MANAGER->push_login_gate_data(buf);
		}

		if (all_empty) {
			//没有数据时候延迟
			Time_Value::sleep(SLEEP_TIME);
		}
	}
}
