/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"

Master_Player::Master_Player(void) { }

Master_Player::~Master_Player(void) { }

int Master_Player::sign_in(Master_Player_Info &player_info) {
	player_info_ = player_info;

	MSG_DEBUG("player sign in master_server. account=[%s], game_cid = %d, player_cid = %d, role_id=%ld, name=%s",
			player_info_.account.c_str(), cid_info_.game_cid, cid_info_.player_cid, player_info_.role_id, player_info_.role_name.c_str());
	return 0;
}

int Master_Player::sign_out(void) {
	reset();

	return 0;
}

void Master_Player::reset(void) {
	recycle_tick_.reset();
	cid_info_.reset();
	player_info_.reset();
}

int Master_Player::tick(Time_Value &now) {
	if (recycle_tick(now) == 1)
		return 0;

	return 0;
}

int Master_Player::recycle_tick(const Time_Value &now) {
	int ret = 0;
	if (now - recycle_tick_.last_tick_ts_ > Recycle_Tick::tick_interval_) {
		recycle_tick_.last_tick_ts_ = now;
		if (recycle_tick_.status_ == Recycle_Tick::RECYCLE && now - recycle_tick_.last_change_status_ts_ > Recycle_Tick::recycle_time_) {
			ret = 1;
			MASTER_MANAGER->unbind_master_player(*this);
			sign_out();
			MASTER_MANAGER->push_master_player(this);
		}
	}
	return ret;
}
