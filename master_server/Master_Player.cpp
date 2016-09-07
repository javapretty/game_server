/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"

Master_Player::Master_Player(void): role_id_(0) { }

Master_Player::~Master_Player(void) { }

int Master_Player::tick(Time_Value &now) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		MASTER_MANAGER->unbind_master_player(*this);
		reset();
		MASTER_MANAGER->push_master_player(this);
	}

	return 0;
}

void Master_Player::reset(void) {
	Player::reset();
	role_id_ = 0;
}

int Master_Player::link_close() {
	if (Player::link_close() < 0) return -1;

	int cid = GET_CID(gate_cid(), player_cid());
	MASTER_MANAGER->push_drop_player_cid(cid);
	return 0;
}
