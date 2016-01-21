#include "Client_Gate_Message.h"


MSG_111000::MSG_111000(void){
	reset();
}

void MSG_111000::serialize(Block_Buffer & w) const {
	w.write_int32(client_time);
}

int MSG_111000::deserialize(Block_Buffer & r) {
	if (r.read_int32(client_time)){
		return -1;
	}
	return 0;
}

void MSG_111000::reset(){
	client_time = 0;
}

MSG_511000::MSG_511000(void){
	reset();
}

void MSG_511000::serialize(Block_Buffer & w) const {
	w.write_int32(client_time);
	w.write_int32(server_time);
}

int MSG_511000::deserialize(Block_Buffer & r) {
	if (r.read_int32(client_time) || r.read_int32(server_time)){
		return -1;
	}
	return 0;
}

void MSG_511000::reset(){
	client_time = 0;
	server_time = 0;
}

MSG_111001::MSG_111001(void){
	reset();
}

void MSG_111001::serialize(Block_Buffer & w) const {
	w.write_string(account);
	w.write_string(session);
}

int MSG_111001::deserialize(Block_Buffer & r) {
	if (r.read_string(account) || r.read_string(session)){
		return -1;
	}
	return 0;
}

void MSG_111001::reset(){
	account.clear();
	session.clear();
}

MSG_511001::MSG_511001(void){
	reset();
}

void MSG_511001::serialize(Block_Buffer & w) const {
	w.write_string(account);
}

int MSG_511001::deserialize(Block_Buffer & r) {
	if (r.read_string(account)){
		return -1;
	}
	return 0;
}

void MSG_511001::reset(){
	account.clear();
}
