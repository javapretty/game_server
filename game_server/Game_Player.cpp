/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Game_Manager.h"

Game_Player::Game_Player(void) { }

Game_Player::~Game_Player(void) { }

int Game_Player::tick(Time_Value &now) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		LOG_INFO("game player recycle, gate_cid:%d, player_cid:%d,", gate_cid(), player_cid());
		GAME_MANAGER->unbind_player(*this);
		reset();
		GAME_MANAGER->push_player(this);
		return 0;
	}
	return 0;
}

int Game_Player::link_close(bool server_close) {
	if (Player::link_close() < 0) return -1;

	int cid = GET_CID(gate_cid(), player_cid());
	GAME_MANAGER->push_drop_player_cid(cid);
	return 0;
}
