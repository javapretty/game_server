#include "Client_Login_Message.h"


MSG_110000::MSG_110000(void){
	reset();
}

void MSG_110000::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(password);
}

int MSG_110000::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	password = buffer.read_string();
	return 0;
}

void MSG_110000::reset(){
	account.clear();
	password.clear();
}

MSG_510000::MSG_510000(void){
	reset();
}

void MSG_510000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(port);
	buffer.write_string(ip);
	buffer.write_string(session);
}

int MSG_510000::deserialize(Block_Buffer &buffer) {
	port = buffer.read_int32();
	ip = buffer.read_string();
	session = buffer.read_string();
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

void MSG_110001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(password);
}

int MSG_110001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	password = buffer.read_string();
	return 0;
}

void MSG_110001::reset(){
	account.clear();
	password.clear();
}

MSG_510001::MSG_510001(void){
	reset();
}

void MSG_510001::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(port);
	buffer.write_string(ip);
	buffer.write_string(session);
}

int MSG_510001::deserialize(Block_Buffer &buffer) {
	port = buffer.read_int32();
	ip = buffer.read_string();
	session = buffer.read_string();
	return 0;
}

void MSG_510001::reset(){
	port = 0;
	ip.clear();
	session.clear();
}
