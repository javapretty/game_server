/*
 * ClientToGame.cpp
 *
 *  Created on: 2015年12月23日
 *      Author: zhangyalei
 */


#include "Client_Game_Message.h"

MSG_120001::MSG_120001(void){
	reset();
}

void MSG_120001::serialize(Block_Buffer& w) const {
	w.write_string(account);
	w.write_int64(role_id);
	w.write_string(timestamp);
	w.write_string(ip);
	w.write_int32(agent_num);
	w.write_int32(server_num);
}

int MSG_120001::deserialize(Block_Buffer& r) {
	if(	r.read_string(account) ||
			r.read_int64(role_id) ||
			r.read_string(timestamp) ||
			r.read_string(ip) ||
			r.read_int32(agent_num) ||
			r.read_int32(server_num)) {
		return -1;
	}
	return 0;
}

void MSG_120001::reset(void) {
	account.clear();
	role_id = 0;
	timestamp.clear();
	ip.clear();
	agent_num = 0;
	server_num = 0;
}

MSG_520001::MSG_520001(void) {
	reset();
}

void MSG_520001::serialize(Block_Buffer& w) const {
	role_info.serialize(w);
}

int MSG_520001::deserialize(Block_Buffer& r) {
	if(role_info.deserialize(r)) {
		return -1;
	}
	return 0;
}

void MSG_520001::reset(void) {
	role_info.reset();
}


MSG_120002::MSG_120002(void) {
	reset();
}

void MSG_120002::serialize(Block_Buffer& w) const {
	w.write_string(account);
	w.write_string(role_name);
	w.write_uint8(gender);
	w.write_int32(agent_num);
	w.write_int32(server_num);
}

int MSG_120002::deserialize(Block_Buffer& r) {
	if(	r.read_string(account) ||
			r.read_string(role_name) ||
			r.read_uint8(gender) ||
			r.read_int32(agent_num) ||
			r.read_int32(server_num)) {
		return -1;
	}
	return 0;
}

void MSG_120002::reset(void) {
	account.clear();
	role_name.clear();
	gender = 0;
	agent_num = 0;
	server_num = 0;
}

MSG_520002::MSG_520002(void) {
	reset();
}

void MSG_520002::serialize(Block_Buffer& w) const {
	w.write_int64(role_id);
}

int MSG_520002::deserialize(Block_Buffer& r) {
	if(r.read_int64(role_id)) {
		return -1;
	}
	return 0;
}

void MSG_520002::reset(void) {
	role_id = 0;
}

