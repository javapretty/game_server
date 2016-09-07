/*
 * Player.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: zhangyalei
 */

#include "Player.h"

Player::Player(void):
	gate_cid_(0),
	game_cid_(0),
	player_cid_(0),
	role_id_(0),
	account_("")
{ }

Player::~Player(void) { }

void Player::reset(void) {
	gate_cid_ = 0;
	game_cid_ = 0;
	player_cid_ = 0;
	role_id_ = 0;
	account_.clear();
	recycle_tick_.reset();
}

int Player::tick(Time_Value &now) {
	return 0;
}

int Player::link_close(bool server_close) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE) return -1;

	recycle_tick_.set(Recycle_Tick::RECYCLE);
	return 0;
}
