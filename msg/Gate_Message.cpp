#include "Gate_Message.h"


MSG_112000::MSG_112000(void){
	reset();
}

void MSG_112000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(gate_port);
	buffer.write_string(account);
	buffer.write_string(session);
	buffer.write_string(gate_ip);
}

int MSG_112000::deserialize(Block_Buffer &buffer) {
	gate_port = buffer.read_int32();
	account = buffer.read_string();
	session = buffer.read_string();
	gate_ip = buffer.read_string();
	return 0;
}

void MSG_112000::reset(){
	gate_port = 0;
	account.clear();
	session.clear();
	gate_ip.clear();
}

MSG_112001::MSG_112001(void){
	reset();
}

void MSG_112001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
}

int MSG_112001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	return 0;
}

void MSG_112001::reset(){
	account.clear();
}
