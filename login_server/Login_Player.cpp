/*
 * Login_Player.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zhangyalei
 */

#include "Login_Manager.h"
#include "Login_Server.h"

Login_Player::Login_Player(void) { }

Login_Player::~Login_Player(void) { }

void Login_Player::reset(void) {
	Player::reset();
	session_info_.reset();
}

int Login_Player::tick(Time_Value &now) {
	if (now - session_info_.session_tick > Time_Value(10)) {
		//session到期，断开客户端与login的连接
		LOG_INFO("player session timeout, drop from login, cid:%d, account:%s", player_cid(), account().c_str());
		session_info_.session_tick += Time_Value(10);
		LOGIN_MANAGER->close_client(0, player_cid(), ERROR_SESSION_TIMEOUT);
	}
	return 0;
}
