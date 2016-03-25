/*
 * Login_Player.cpp
 *
 *  Created on: 2016年1月21日
 *      Author: zhangyalei
 */

#include "Login_Manager.h"

Login_Player::Login_Player(void) : cid_(0) { }

Login_Player::~Login_Player(void) { }

void Login_Player::reset(void) {
	cid_ = -1;
	recycle_tick_.reset();
}

int Login_Player::tick(Time_Value &now) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		LOGIN_MANAGER->unbind_login_player(*this);
		reset();
		LOGIN_MANAGER->push_login_player(this);
	}
	return 0;
}
