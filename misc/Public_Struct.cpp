/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "Configurator.h"
#include "DB_Operator.h"

const Time_Value Recycle_Tick::recycle_time_ = Time_Value(2, 0);
const Time_Value Recycle_Tick::tick_interval_ = Time_Value(2, 0);


Account_Info::Account_Info(void) { reset(); }

int Account_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	return 0;
}

int Account_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_string(account);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	return 0;
}

void Account_Info::reset(void) {
	account.clear();
	agent_num = 0;
	server_num = 0;
}

Gate_Player_Info::Gate_Player_Info(void) { reset(); }

int Gate_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	return 0;
}

int Gate_Player_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(account);
	return 0;
}

void Gate_Player_Info::reset(void) {
	role_id = 0;
	account.clear();
}

Master_Player_Info::Master_Player_Info(void) { reset(); }

int Master_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(level);
	return 0;
}

int Master_Player_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_int32(level);
	return 0;
}

void Master_Player_Info::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	level = 0;
}

Game_Player_Info::Game_Player_Info(void) { reset(); }

int Game_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_uint8(gender);
	buffer.write_int32(level);
	buffer.write_uint8(career);

	buffer.write_int32(create_time);
	buffer.write_int32(last_sign_in_time);
	buffer.write_int32(last_sign_out_time);
	buffer.write_string(ip);

	return 0;
}

int Game_Player_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_uint8((uint8_t &)gender);
	buffer.read_int32(level);
	buffer.read_uint8((uint8_t &)career);
	buffer.read_int32(create_time);
	buffer.read_int32(last_sign_in_time);
	buffer.read_int32(last_sign_out_time);
	buffer.read_string(ip);
	return 0;
}

int Game_Player_Info::load(void) {
	return CACHED_INSTANCE->load_player_info(*this);
}

int Game_Player_Info::save(void) {
	return CACHED_INSTANCE->save_player_info(*this);
}

void Game_Player_Info::reset(void) {
	role_id = 0;
	agent_num = 0;
	server_num = 0;
	account.clear();
	role_name.clear();
	gender = 0;
	career = 0;
	create_time = 0;
	level = 0;
	last_sign_in_time = 0;
	last_sign_out_time = 0;
	ip.clear();
}

Player_Data::Player_Data(void) { reset(); }

int Player_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int8(status);

	game_player_info.serialize(buffer);
	return 0;
}

int Player_Data::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_int8(status);

	game_player_info.deserialize(buffer);
	return 0;
}

int Player_Data::load() {
	game_player_info.role_id = role_id;
	game_player_info.load();
	return 0;
}

int Player_Data::save(void) {
	game_player_info.save();
	return 0;
}

void Player_Data::reset(void) {
	role_id = 0;
	status =	NULL_STATUS;

	game_player_info.reset();
}

int Player_DB_Cache::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_int32(gender);
	buffer.write_int32(career);
	buffer.write_int32(level);
	return 0;
}

int Player_DB_Cache::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	buffer.read_int32(gender);
	buffer.read_int32(career);
	buffer.read_int32(level);
	return 0;
}

void Player_DB_Cache::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	agent_num = 0;
	server_num = 0;
	gender = 0;
	career = 0;
	level = 0;
}
