/*
 * MsgStruct.cpp
 *
 *  Created on: 2016年1月8日
 *      Author: zhangyalei
 */

#include "Msg_Struct.h"

Role_Info::Role_Info(void) {
	reset();
}

void Role_Info::serialize(Block_Buffer& w) const {
	w.write_int64(role_id);
	w.write_string(account);
	w.write_string(role_name);
	w.write_uint8(career);
	w.write_uint8(gender);
	w.write_uint16(level);
}

int Role_Info::deserialize(Block_Buffer& r) {
	if(	r.read_int64(role_id) ||
			r.read_string(account) ||
			r.read_string(role_name) ||
			r.read_uint8(career) ||
			r.read_uint8(gender) ||
			r.read_uint16(level)) {
		return -1;
	}
	return 0;
}

void Role_Info::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	career = 0;
	gender = 0;
	level = 0;
}

