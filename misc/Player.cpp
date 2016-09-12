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
}

int Player::tick(Time_Value &now) {
	return 0;
}
