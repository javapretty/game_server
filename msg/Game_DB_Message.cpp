/*
 * GameDBMessage.cpp
 *
 *  Created on: 2016年1月8日
 *      Author: zhangyalei
 */

#include "Game_DB_Message.h"

MSG_150000::MSG_150000(void){
	reset();
}

void MSG_150000::serialize(Block_Buffer& w) const { }

int MSG_150000::deserialize(Block_Buffer& r) {
	return 0;
}

void MSG_150000::reset(void) { }

MSG_550000::MSG_550000(void) {
	reset();
}

void MSG_550000::serialize(Block_Buffer& w) const {
	w.write_uint16(cache_vector.size());
	for (std::vector<Player_DB_Cache>::const_iterator iter = cache_vector.begin(); iter != cache_vector.end(); ++iter) {
		(*iter).serialize(w);
	}
}

int MSG_550000::deserialize(Block_Buffer& r) {
	uint16_t len = 0;
	r.read_uint16(len);
	Player_DB_Cache db_cache;
	for (int i = 0; i < len; ++i) {
		db_cache.reset();
		db_cache.deserialize(r);
		cache_vector.push_back(db_cache);
	}
	return 0;
}

void MSG_550000::reset() {
	cache_vector.clear();
}

MSG_150001::MSG_150001(void){
	reset();
}

void MSG_150001::serialize(Block_Buffer& w) const {
	account_info.serialize(w);
}

int MSG_150001::deserialize(Block_Buffer& r) {
	if(account_info.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_150001::reset(void) {
	account_info.reset();
}

MSG_550001::MSG_550001(void) {
	reset();
}

void MSG_550001::serialize(Block_Buffer& w) const {
	player_data.serialize(w);
}

int MSG_550001::deserialize(Block_Buffer& r) {
	if(player_data.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_550001::reset(void) {
	player_data.reset();
}

MSG_150002::MSG_150002(void) {
	reset();
}

void MSG_150002::serialize(Block_Buffer& w) const {
	player_info.serialize(w);
}

int MSG_150002::deserialize(Block_Buffer& r) {
	if(player_info.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_150002::reset(void) {
	player_info.reset();
}

MSG_550002::MSG_550002(void) {
	reset();
}

void MSG_550002::serialize(Block_Buffer& w) const {
	player_data.serialize(w);
}

int MSG_550002::deserialize(Block_Buffer& r) {
	if(player_data.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_550002::reset(void) {
	player_data.reset();
}

MSG_150003::MSG_150003(void) {
	reset();
}

void MSG_150003::serialize(Block_Buffer& w) const {
	player_data.serialize(w);
}

int MSG_150003::deserialize(Block_Buffer& r) {
	if(player_data.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_150003::reset(void) {
	player_data.reset();
}

MSG_550003::MSG_550003(void) {
	reset();
}

void MSG_550003::serialize(Block_Buffer& w) const {
	w.write_int64(role_id);
}

int MSG_550003::deserialize(Block_Buffer& r) {
	if(r.read_int64(role_id)) {
		return -1;
	}
	return 0;
}

void MSG_550003::reset(void) {
	role_id = 0;
}
