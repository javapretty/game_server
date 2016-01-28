#include "Gate_Message.h"


MSG_112000::MSG_112000(void){
	reset();
}

void MSG_112000::serialize(Block_Buffer & w) const {
	w.write_int32(gate_port);
	w.write_string(account);
	w.write_string(session);
	w.write_string(gate_ip);
}

int MSG_112000::deserialize(Block_Buffer & r) {
	gate_port = r.read_int32();
	account = r.read_string();
	session = r.read_string();
	gate_ip = r.read_string();
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

void MSG_112001::serialize(Block_Buffer & w) const {
	w.write_string(account);
}

int MSG_112001::deserialize(Block_Buffer & r) {
	account = r.read_string();
	return 0;
}

void MSG_112001::reset(){
	account.clear();
}
