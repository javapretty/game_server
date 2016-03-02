#include "Client_Login_Message.h"


MSG_100000::MSG_100000(void){
	reset();
}

void MSG_100000::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(password);
}

int MSG_100000::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	password = buffer.read_string();
	return 0;
}

void MSG_100000::reset(){
	account.clear();
	password.clear();
}

MSG_500000::MSG_500000(void){
	reset();
}

void MSG_500000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(port);
	buffer.write_string(ip);
	buffer.write_string(session);
}

int MSG_500000::deserialize(Block_Buffer &buffer) {
	port = buffer.read_int32();
	ip = buffer.read_string();
	session = buffer.read_string();
	return 0;
}

void MSG_500000::reset(){
	port = 0;
	ip.clear();
	session.clear();
}

MSG_100001::MSG_100001(void){
	reset();
}

void MSG_100001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(password);
}

int MSG_100001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	password = buffer.read_string();
	return 0;
}

void MSG_100001::reset(){
	account.clear();
	password.clear();
}

MSG_500001::MSG_500001(void){
	reset();
}

void MSG_500001::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(port);
	buffer.write_string(ip);
	buffer.write_string(session);
}

int MSG_500001::deserialize(Block_Buffer &buffer) {
	port = buffer.read_int32();
	ip = buffer.read_string();
	session = buffer.read_string();
	return 0;
}

void MSG_500001::reset(){
	port = 0;
	ip.clear();
	session.clear();
}

MSG_100002::MSG_100002(void){
	reset();
}

void MSG_100002::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(session);
}

int MSG_100002::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	session = buffer.read_string();
	return 0;
}

void MSG_100002::reset(){
	account.clear();
	session.clear();
}

MSG_500002::MSG_500002(void){
	reset();
}

void MSG_500002::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
}

int MSG_500002::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	return 0;
}

void MSG_500002::reset(){
	account.clear();
}

MSG_100003::MSG_100003(void){
	reset();
}

void MSG_100003::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(client_time);
}

int MSG_100003::deserialize(Block_Buffer &buffer) {
	client_time = buffer.read_int32();
	return 0;
}

void MSG_100003::reset(){
	client_time = 0;
}

MSG_500003::MSG_500003(void){
	reset();
}

void MSG_500003::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(client_time);
	buffer.write_int32(server_time);
}

int MSG_500003::deserialize(Block_Buffer &buffer) {
	client_time = buffer.read_int32();
	server_time = buffer.read_int32();
	return 0;
}

void MSG_500003::reset(){
	client_time = 0;
	server_time = 0;
}
