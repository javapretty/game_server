/*
 * Game_Master_Message.cpp
 *
 *  Created on: 2016年1月11日
 *      Author: zhangyalei
 */

#include "Game_Master_Message.h"

MSG_160001::MSG_160001(void){
	reset();
}

void MSG_160001::serialize(Block_Buffer& w) const {
	player_info.serialize(w);
}

int MSG_160001::deserialize(Block_Buffer& r) {
	if(player_info.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_160001::reset(void) {
	player_info.reset();
}

MSG_160002::MSG_160002(void){
	reset();
}

void MSG_160002::serialize(Block_Buffer& w) const {
	w.write_int64(role_id);
}

int MSG_160002::deserialize(Block_Buffer& r) {
	if(r.read_int64(role_id)) {
		return -1;
	}
	return 0;
}

void MSG_160002::reset(void) {
	role_id = 0;
}

