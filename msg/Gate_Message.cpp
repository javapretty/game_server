#include "Gate_Message.h"


MSG_140000::MSG_140000(void){
	reset();
}

void MSG_140000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(gate_port);
	buffer.write_string(account);
	buffer.write_string(session);
	buffer.write_string(gate_ip);
}

int MSG_140000::deserialize(Block_Buffer &buffer) {
	gate_port = buffer.read_int32();
	account = buffer.read_string();
	session = buffer.read_string();
	gate_ip = buffer.read_string();
	return 0;
}

void MSG_140000::reset(){
	gate_port = 0;
	account.clear();
	session.clear();
	gate_ip.clear();
}

MSG_140001::MSG_140001(void){
	reset();
}

void MSG_140001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
}

int MSG_140001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	return 0;
}

void MSG_140001::reset(){
	account.clear();
}
