/*
 * Inner_Msg.cpp
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#include "Inner_Msg.h"

Create_Role_Info::Create_Role_Info(void) {
	reset();
}

Create_Role_Info::~Create_Role_Info() {
}

void Create_Role_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int8(gender);
	buffer.write_int8(career);
}

int Create_Role_Info::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	role_name = buffer.read_string();
	gender = buffer.read_int8();
	career = buffer.read_int8();
	return 0;
}

void Create_Role_Info::reset(void) {
	account.clear();
	role_name.clear();
	gender = 0;
	career = 0;
}

void Create_Role_Info::print(void) {
	printf("account: %s, ", account.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("gender: %d, ", gender);
	printf("career: %d, ", career);
	printf("\n");
}

Create_Guild_Info::Create_Guild_Info(void) {
	reset();
}

Create_Guild_Info::~Create_Guild_Info() {
}

void Create_Guild_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(guild_id);
	buffer.write_string(guild_name);
	buffer.write_int64(chief_id);
}

int Create_Guild_Info::deserialize(Block_Buffer &buffer) {
	guild_id = buffer.read_int64();
	guild_name = buffer.read_string();
	chief_id = buffer.read_int64();
	return 0;
}

void Create_Guild_Info::reset(void) {
	guild_id = 0;
	guild_name.clear();
	chief_id = 0;
}

void Create_Guild_Info::print(void) {
	printf("guild_id: %ld, ", guild_id);
	printf("guild_name: %s, ", guild_name.c_str());
	printf("chief_id: %ld, ", chief_id);
	printf("\n");
}

MSG_100000::MSG_100000(void) {
	reset();
}

MSG_100000::~MSG_100000() {
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

void MSG_100000::reset(void) {
	account.clear();
	password.clear();
}

void MSG_100000::print(void) {
	printf("account: %s, ", account.c_str());
	printf("password: %s, ", password.c_str());
	printf("\n");
}

MSG_500000::MSG_500000(void) {
	reset();
}

MSG_500000::~MSG_500000() {
}

void MSG_500000::serialize(Block_Buffer &buffer) const {
	buffer.write_string(ip);
	buffer.write_int32(port);
	buffer.write_string(session);
}

int MSG_500000::deserialize(Block_Buffer &buffer) {
	ip = buffer.read_string();
	port = buffer.read_int32();
	session = buffer.read_string();
	return 0;
}

void MSG_500000::reset(void) {
	ip.clear();
	port = 0;
	session.clear();
}

void MSG_500000::print(void) {
	printf("ip: %s, ", ip.c_str());
	printf("port: %d, ", port);
	printf("session: %s, ", session.c_str());
	printf("\n");
}

MSG_100001::MSG_100001(void) {
	reset();
}

MSG_100001::~MSG_100001() {
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

void MSG_100001::reset(void) {
	account.clear();
	password.clear();
}

void MSG_100001::print(void) {
	printf("account: %s, ", account.c_str());
	printf("password: %s, ", password.c_str());
	printf("\n");
}

MSG_500001::MSG_500001(void) {
	reset();
}

MSG_500001::~MSG_500001() {
}

void MSG_500001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(ip);
	buffer.write_int32(port);
	buffer.write_string(session);
}

int MSG_500001::deserialize(Block_Buffer &buffer) {
	ip = buffer.read_string();
	port = buffer.read_int32();
	session = buffer.read_string();
	return 0;
}

void MSG_500001::reset(void) {
	ip.clear();
	port = 0;
	session.clear();
}

void MSG_500001::print(void) {
	printf("ip: %s, ", ip.c_str());
	printf("port: %d, ", port);
	printf("session: %s, ", session.c_str());
	printf("\n");
}

MSG_100002::MSG_100002(void) {
	reset();
}

MSG_100002::~MSG_100002() {
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

void MSG_100002::reset(void) {
	account.clear();
	session.clear();
}

void MSG_100002::print(void) {
	printf("account: %s, ", account.c_str());
	printf("session: %s, ", session.c_str());
	printf("\n");
}

MSG_500002::MSG_500002(void) {
	reset();
}

MSG_500002::~MSG_500002() {
}

void MSG_500002::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
}

int MSG_500002::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	return 0;
}

void MSG_500002::reset(void) {
	account.clear();
}

void MSG_500002::print(void) {
	printf("account: %s, ", account.c_str());
	printf("\n");
}

MSG_100003::MSG_100003(void) {
	reset();
}

MSG_100003::~MSG_100003() {
}

void MSG_100003::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(client_time);
}

int MSG_100003::deserialize(Block_Buffer &buffer) {
	client_time = buffer.read_int32();
	return 0;
}

void MSG_100003::reset(void) {
	client_time = 0;
}

void MSG_100003::print(void) {
	printf("client_time: %d, ", client_time);
	printf("\n");
}

MSG_500003::MSG_500003(void) {
	reset();
}

MSG_500003::~MSG_500003() {
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

void MSG_500003::reset(void) {
	client_time = 0;
	server_time = 0;
}

void MSG_500003::print(void) {
	printf("client_time: %d, ", client_time);
	printf("server_time: %d, ", server_time);
	printf("\n");
}

MSG_140000::MSG_140000(void) {
	reset();
}

MSG_140000::~MSG_140000() {
}

void MSG_140000::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(session);
	buffer.write_string(gate_ip);
	buffer.write_int32(gate_port);
}

int MSG_140000::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	session = buffer.read_string();
	gate_ip = buffer.read_string();
	gate_port = buffer.read_int32();
	return 0;
}

void MSG_140000::reset(void) {
	account.clear();
	session.clear();
	gate_ip.clear();
	gate_port = 0;
}

void MSG_140000::print(void) {
	printf("account: %s, ", account.c_str());
	printf("session: %s, ", session.c_str());
	printf("gate_ip: %s, ", gate_ip.c_str());
	printf("gate_port: %d, ", gate_port);
	printf("\n");
}

MSG_140001::MSG_140001(void) {
	reset();
}

MSG_140001::~MSG_140001() {
}

void MSG_140001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
}

int MSG_140001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	return 0;
}

void MSG_140001::reset(void) {
	account.clear();
}

void MSG_140001::print(void) {
	printf("account: %s, ", account.c_str());
	printf("\n");
}
