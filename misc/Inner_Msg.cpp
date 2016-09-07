/*
 * Inner_Msg.cpp
 *
 *  Created on: Aug 2, 2016
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
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_int8(gender);
	buffer.read_int8(career);
	return 0;
}

void Create_Role_Info::reset(void) {
	account.clear();
	role_name.clear();
	gender = 0;
	career = 0;
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
	buffer.read_int64(guild_id);
	buffer.read_string(guild_name);
	buffer.read_int64(chief_id);
	return 0;
}

void Create_Guild_Info::reset(void) {
	guild_id = 0;
	guild_name.clear();
	chief_id = 0;
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
	buffer.read_string(account);
	buffer.read_string(password);
	return 0;
}

void MSG_100001::reset(void) {
	account.clear();
	password.clear();
}

MSG_500001::MSG_500001(void) {
	reset();
}

MSG_500001::~MSG_500001() {
}

void MSG_500001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(gate_ip);
	buffer.write_int32(gate_port);
	buffer.write_string(session);
}

int MSG_500001::deserialize(Block_Buffer &buffer) {
	buffer.read_string(gate_ip);
	buffer.read_int32(gate_port);
	buffer.read_string(session);
	return 0;
}

void MSG_500001::reset(void) {
	gate_ip.clear();
	gate_port = 0;
	session.clear();
}

MSG_100101::MSG_100101(void) {
	reset();
}

MSG_100101::~MSG_100101() {
}

void MSG_100101::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(session);
}

int MSG_100101::deserialize(Block_Buffer &buffer) {
	buffer.read_string(account);
	buffer.read_string(session);
	return 0;
}

void MSG_100101::reset(void) {
	account.clear();
	session.clear();
}

MSG_500101::MSG_500101(void) {
	reset();
}

MSG_500101::~MSG_500101() {
}

void MSG_500101::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
}

int MSG_500101::deserialize(Block_Buffer &buffer) {
	buffer.read_string(account);
	return 0;
}

void MSG_500101::reset(void) {
	account.clear();
}

MSG_100102::MSG_100102(void) {
	reset();
}

MSG_100102::~MSG_100102() {
}

void MSG_100102::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(client_time);
}

int MSG_100102::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(client_time);
	return 0;
}

void MSG_100102::reset(void) {
	client_time = 0;
}

MSG_500102::MSG_500102(void) {
	reset();
}

MSG_500102::~MSG_500102() {
}

void MSG_500102::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(client_time);
	buffer.write_int32(server_time);
}

int MSG_500102::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(client_time);
	buffer.read_int32(server_time);
	return 0;
}

void MSG_500102::reset(void) {
	client_time = 0;
	server_time = 0;
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
	buffer.read_string(account);
	buffer.read_string(session);
	buffer.read_string(gate_ip);
	buffer.read_int32(gate_port);
	return 0;
}

void MSG_140000::reset(void) {
	account.clear();
	session.clear();
	gate_ip.clear();
	gate_port = 0;
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
	buffer.read_string(account);
	return 0;
}

void MSG_140001::reset(void) {
	account.clear();
}
