/** 
* struct Log_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#include "Log_Message.h"

MSG_180000::MSG_180000(void) {
	reset();
}

void MSG_180000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(id);
	buffer.write_string(name);
}

int MSG_180000::deserialize(Block_Buffer &buffer) {
	id = buffer.read_int32();
	name = buffer.read_string();
	return 0;
}

void MSG_180000::reset(void) {
	id = 0;
	name.clear();
}

MSG_180001::MSG_180001(void) {
	reset();
}

void MSG_180001::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(role_name);
	buffer.write_string(account);
	buffer.write_uint16(level);
	buffer.write_string(client_ip);
	buffer.write_uint32(login_time);
	buffer.write_uint32(logout_time);
	buffer.write_uint32(online_time);
}

int MSG_180001::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	role_name = buffer.read_string();
	account = buffer.read_string();
	level = buffer.read_uint16();
	client_ip = buffer.read_string();
	login_time = buffer.read_uint32();
	logout_time = buffer.read_uint32();
	online_time = buffer.read_uint32();
	return 0;
}

void MSG_180001::reset(void) {
	role_id = 0;
	role_name.clear();
	account.clear();
	level = 0;
	client_ip.clear();
	login_time = 0;
	logout_time = 0;
	online_time = 0;
}
