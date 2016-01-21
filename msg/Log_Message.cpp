#include "Log_Message.h"


MSG_180000::MSG_180000(void){
	reset();
}

void MSG_180000::serialize(Block_Buffer & w) const {
	w.write_int32(pid);
	w.write_int64(tid);
	w.write_uint8(log_type);
	w.write_uint8(log_sub_type);
	w.write_uint32(time);
	w.write_string(log_str);
}

int MSG_180000::deserialize(Block_Buffer & r) {
	if (r.read_int32(pid) || r.read_int64(tid) || r.read_uint8(log_type) || r.read_uint8(log_sub_type) || r.read_uint32(time) || r.read_string(log_str)){
		return -1;
	}
	return 0;
}

void MSG_180000::reset(){
	pid = 0;
	tid = 0;
	log_type = 0;
	log_sub_type = 0;
	time = 0;
	log_str.clear();
}

MSG_180001::MSG_180001(void){
	reset();
}

void MSG_180001::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
	w.write_uint8(log_type);
	w.write_uint8(log_sub_type);
	w.write_string(file_name);
	w.write_string(buf);
}

int MSG_180001::deserialize(Block_Buffer & r) {
	if (r.read_int64(role_id) || r.read_uint8(log_type) || r.read_uint8(log_sub_type) || r.read_string(file_name) || r.read_string(buf)){
		return -1;
	}
	return 0;
}

void MSG_180001::reset(){
	role_id = 0;
	log_type = 0;
	log_sub_type = 0;
	file_name.clear();
	buf.clear();
}

MSG_185000::MSG_185000(void){
	reset();
}

void MSG_185000::serialize(Block_Buffer & w) const {
	w.write_int32(id);
	w.write_string(name);
}

int MSG_185000::deserialize(Block_Buffer & r) {
	if (r.read_int32(id) || r.read_string(name)){
		return -1;
	}
	return 0;
}

void MSG_185000::reset(){
	id = 0;
	name.clear();
}

MSG_185001::MSG_185001(void){
	reset();
}

void MSG_185001::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
	w.write_uint16(level);
	w.write_uint32(login_time);
	w.write_uint32(logout_time);
	w.write_uint32(online_time);
	w.write_string(role_name);
	w.write_string(account);
	w.write_string(client_ip);
}

int MSG_185001::deserialize(Block_Buffer & r) {
	if (r.read_int64(role_id) || r.read_uint16(level) || r.read_uint32(login_time) || r.read_uint32(logout_time) || r.read_uint32(online_time) || r.read_string(role_name) || r.read_string(account) || r.read_string(client_ip)){
		return -1;
	}
	return 0;
}

void MSG_185001::reset(){
	role_id = 0;
	level = 0;
	login_time = 0;
	logout_time = 0;
	online_time = 0;
	role_name.clear();
	account.clear();
	client_ip.clear();
}
