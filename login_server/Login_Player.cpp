/*
 * Login_Player.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zhangyalei
 */

#include "Login_Manager.h"

Login_Player::Login_Player(void) { }

Login_Player::~Login_Player(void) { }

int Login_Player::tick(Time_Value &now) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		LOGIN_MANAGER->unbind_login_player(*this);
		reset();
		LOGIN_MANAGER->push_login_player(this);
	}
	return 0;
}

void Login_Player::reset(void) {
	Player::reset();
	player_info_.reset();
}

int Login_Player::link_close() {
	if (Player::link_close() < 0) return -1;

	//修改session_tick的值，防止login_manager对同一个链接重复push_drop
	player_info_.session_tick += 10;
	return 0;
}
