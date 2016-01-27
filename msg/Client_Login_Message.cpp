#include "Client_Login_Message.h"


MSG_110000::MSG_110000(void){
	reset();
}

void MSG_110000::serialize(Block_Buffer & w) const {
	w.write_string(account);
	w.write_string(password);
}

int MSG_110000::deserialize(Block_Buffer & r) {
	account = r.read_string();
	password = r.read_string();
	return 0;
}

void MSG_110000::reset(){
	account.clear();
	password.clear();
}

MSG_510000::MSG_510000(void){
	reset();
}

void MSG_510000::serialize(Block_Buffer & w) const {
	w.write_int32(port);
	w.write_string(ip);
	w.write_string(session);
}

int MSG_510000::deserialize(Block_Buffer & r) {
	port = r.read_int32();
	ip = r.read_string();
	session = r.read_string();
	return 0;
}

void MSG_510000::reset(){
	port = 0;
	ip.clear();
	session.clear();
}

MSG_110001::MSG_110001(void){
	reset();
}

void MSG_110001::serialize(Block_Buffer & w) const {
	w.write_string(account);
	w.write_string(password);
}

int MSG_110001::deserialize(Block_Buffer & r) {
	account = r.read_string();
	password = r.read_string();
	return 0;
}

void MSG_110001::reset(){
	account.clear();
	password.clear();
}

MSG_510001::MSG_510001(void){
	reset();
}

void MSG_510001::serialize(Block_Buffer & w) const {
	w.write_int32(port);
	w.write_string(ip);
	w.write_string(session);
}

int MSG_510001::deserialize(Block_Buffer & r) {
	port = r.read_int32();
	ip = r.read_string();
	session = r.read_string();
	return 0;
}

void MSG_510001::reset(){
	port = 0;
	ip.clear();
	session.clear();
}
