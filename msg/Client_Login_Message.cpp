/*
 * ClientToLogin.cpp
 *
 *  Created on: 2016年1月6日
 *      Author: zhangyalei
 */

#include "Client_Login_Message.h"

MSG_110000::MSG_110000(void){
	reset();
}

void MSG_110000::serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_string(password);
}

int MSG_110000::deserialize(Block_Buffer & r) {
	if( r.read_string(account)  ||  r.read_string(password))
			return -1;
		return 0;
}

void MSG_110000::reset(){
	account.clear();
	password.clear();
}

//MSG_510001
MSG_510000::MSG_510000(void){
	reset();
}

void MSG_510000::serialize(Block_Buffer & w) const {
		w.write_string(ip);
		w.write_int32(port);
		w.write_string(session);
}

int MSG_510000::deserialize(Block_Buffer & r) {
	if( r.read_string(ip)  ||  r.read_int32(port) || r.read_string(session))
			return -1;
		return 0;
}

void MSG_510000::reset(){
	ip.clear();
	port = 0;
	session.clear();
}

//MSG_110001
MSG_110001::MSG_110001(void){
	reset();
}

void MSG_110001::serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_string(password);
}

int MSG_110001::deserialize(Block_Buffer & r) {
	if( r.read_string(account)  ||  r.read_string(password))
			return -1;
		return 0;
}

void MSG_110001::reset(){
	account.clear();
	password.clear();
}

//MSG_510001
MSG_510001::MSG_510001(void){
	reset();
}

void MSG_510001::serialize(Block_Buffer & w) const {
		w.write_string(ip);
		w.write_int32(port);
		w.write_string(session);
}

int MSG_510001::deserialize(Block_Buffer & r) {
	if( r.read_string(ip)  ||  r.read_int32(port) || r.read_string(session))
			return -1;
		return 0;
}

void MSG_510001::reset(){
	ip.clear();
	port = 0;
	session.clear();
}
