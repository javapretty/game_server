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
	if (recycle_tick(now) == 1)
		return 0;

	return 0;
}

int Login_Player::recycle_tick(const Time_Value &now) {
	int ret = 0;
	if (now - recycle_tick_.last_tick_ts_ > Recycle_Tick::tick_interval_) {
		recycle_tick_.last_tick_ts_ = now;
		if (recycle_tick_.status_ == Recycle_Tick::RECYCLE && now - recycle_tick_.last_change_status_ts_ > Recycle_Tick::recycle_time_) {
			ret = 1;
			LOGIN_MANAGER->unbind_login_player(*this);
			reset();
			LOGIN_MANAGER->push_login_player(this);
		}
	}
	return ret;
}
