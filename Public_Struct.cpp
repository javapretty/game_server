/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "Server_Config.h"
#include "DB_Operator.h"

const Time_Value Recycle_Tick::recycle_time_ = Time_Value(2, 0);
const Time_Value Recycle_Tick::tick_interval_ = Time_Value(2, 0);
const Time_Value Recycle_Tick::session_interval_ = Time_Value(30, 0);

Account_Info::Account_Info(void) { reset(); }

int Account_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	return 0;
}

int Account_Info::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	return 0;
}

void Account_Info::reset(void) {
	account.clear();
	agent_num = 0;
	server_num = 0;
}

Login_Player_Info::Login_Player_Info(void){ reset(); }

int Login_Player_Info::serialize(Block_Buffer &buffer) const {
	session_tick.serialize(buffer);
	buffer.write_string(account);
	buffer.write_string(gate_ip);
	buffer.write_int32(gate_port);
	buffer.write_string(session);
	return 0;
}

int Login_Player_Info::deserialize(Block_Buffer &buffer) {
	session_tick.deserialize(buffer);
	account = buffer.read_string();
	gate_ip = buffer.read_string();
	gate_port = buffer.read_int32();
	session = buffer.read_string();
	return 0;
}

void Login_Player_Info::reset(void) {
	session_tick = Time_Value::gettimeofday();
	account.clear();
	gate_ip.clear();
	gate_port = 0;
	session.clear();
}

Gate_Player_Info::Gate_Player_Info(void) { reset(); }

int Gate_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	return 0;
}

int Gate_Player_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	account = buffer.read_string();
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
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	level = buffer.read_int32();
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
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_string(client_ip);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(gender);
	buffer.write_int32(career);
	buffer.write_int32(create_time);
	buffer.write_int32(last_sign_in_time);
	buffer.write_int32(last_sign_out_time);
	buffer.write_int32(vitality);
	buffer.write_int32(vip);
	buffer.write_int32(charge_gold);
	buffer.write_bool(is_change);
	return 0;
}

int Game_Player_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	client_ip = buffer.read_string();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	level = buffer.read_int32();
	exp = buffer.read_int32();
	gender = buffer.read_int32();
	career = buffer.read_int32();
	create_time = buffer.read_int32();
	last_sign_in_time = buffer.read_int32();
	last_sign_out_time = buffer.read_int32();
	vitality = buffer.read_int32();
	vip = buffer.read_int32();
	charge_gold = buffer.read_int32();
	is_change = buffer.read_bool();
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
	account.clear();
	role_name.clear();
	client_ip.clear();
	agent_num = 0;
	server_num = 0;
	level = 0;
	exp = 0;
	gender = 0;
	career = 0;
	create_time = 0;
	last_sign_in_time = 0;
	last_sign_out_time = 0;
	vitality = 0;
	vip = 0;
	charge_gold = 0;
	is_change = false;
}

Hero_Info::Hero_Info(void) { reset(); }

int Hero_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);

	buffer.write_uint16(hero_map.size());
	for (Hero_Map::const_iterator it = hero_map.begin(); it != hero_map.end(); ++it) {
		it->second.serialize(buffer);
	}
	buffer.write_bool(is_change);
	return 0;
}

int Hero_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();

	uint16_t size = buffer.read_uint16();
	Hero_Detail hero_detail;
	for (int i = 0; i < size; ++i) {
		hero_detail.deserialize(buffer);
		hero_map[hero_detail.hero_id] = hero_detail;
	}
	is_change = buffer.read_bool();
	return 0;
}

int Hero_Info::load(void) {
	return CACHED_INSTANCE->load_hero_info(*this);
}

int Hero_Info::save(void) {
	if (is_change)
		return CACHED_INSTANCE->save_hero_info(*this);
	else
		return 0;
}

void Hero_Info::reset(void) {
	role_id = 0;
	hero_map.clear();
	is_change = false;
}

Item_Info::Item_Info(void) {
	reset();
}

int Item_Info::serialize(Block_Buffer &buffer) const {
	item_basic.serialize(buffer);
	return 0;
}

int Item_Info::deserialize(Block_Buffer &buffer){
	item_basic.deserialize(buffer);
	return 0;
}

void Item_Info::reset(void){
	item_basic.reset();
}

Bag_Info::Bag_Info(void) { reset(); }

int Bag_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int32(copper);
	buffer.write_int32(gold);

	buffer.write_uint16(item_map.size());
	for (Item_Map::const_iterator it = item_map.begin(); it != item_map.end(); ++it) {
		it->second.serialize(buffer);
	}
	buffer.write_bool(is_change);
	return 0;
}

int Bag_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	copper = buffer.read_int32();
	gold = buffer.read_int32();

	uint16_t size = buffer.read_uint16();
	Item_Info item_info;
	for (int i = 0; i < size; ++i) {
		item_info.deserialize(buffer);
		item_map[item_info.item_basic.id] = item_info;
	}
	is_change = buffer.read_bool();
	return 0;
}

int Bag_Info::load(void) {
	return CACHED_INSTANCE->load_bag_info(*this);
}

int Bag_Info::save(void) {
	if (is_change)
		return CACHED_INSTANCE->save_bag_info(*this);
	else
		return 0;
}

void Bag_Info::reset(void) {
	role_id = 0;
	copper = 0;
	gold = 0;
	item_map.clear();
	is_change = false;
}

Mail_Info::Mail_Info() { reset(); }

int Mail_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int32(total_count);

	buffer.write_uint16(mail_map.size());
	for(Mail_Map::const_iterator it = mail_map.begin();
			it != mail_map.end(); ++it) {
		it->second.serialize(buffer);
	}

	buffer.write_bool(is_change);
	return 0;
}

int Mail_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	total_count = buffer.read_int32();

	uint16_t size = buffer.read_uint16();
	Mail_Detail mail_detail;
	for (int16_t i = 0; i < size; ++i) {
		mail_detail.deserialize(buffer);
		mail_map.insert(std::make_pair(mail_detail.mail_id, mail_detail));
	}

	is_change = buffer.read_bool();
	return 0;
}

int Mail_Info::load(void) {
	return CACHED_INSTANCE->load_mail_info(*this);
}

int Mail_Info::save(void) {
	if (is_change)
		return CACHED_INSTANCE->save_mail_info(*this);
	else
		return 0;
}

void Mail_Info::reset(void) {
	role_id = 0;
	total_count = 0;
	mail_map.clear();
	is_change = false;
}

Player_Data::Player_Data(void) { reset(); }

int Player_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int8(status);

	game_player_info.serialize(buffer);
	hero_info.serialize(buffer);
	bag_info.serialize(buffer);
	mail_info.serialize(buffer);
	return 0;
}

int Player_Data::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	status = buffer.read_int8();

	game_player_info.deserialize(buffer);
	hero_info.deserialize(buffer);
	bag_info.deserialize(buffer);
	mail_info.deserialize(buffer);
	return 0;
}

int Player_Data::load() {
	game_player_info.role_id = role_id;
	game_player_info.load();
	hero_info.load();
	bag_info.load();
	mail_info.load();
	return 0;
}

int Player_Data::save(void) {
	game_player_info.save();
	hero_info.save();
	bag_info.save();
	mail_info.save();
	return 0;
}

void Player_Data::reset(void) {
	role_id = 0;
	status =	NULL_STATUS;

	game_player_info.reset();
	hero_info.reset();
	bag_info.reset();
	mail_info.reset();
}

bool Player_Data::can_save(void) {
	return game_player_info.is_change || bag_info.is_change || mail_info.is_change;
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
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	gender = buffer.read_int32();
	career = buffer.read_int32();
	level = buffer.read_int32();
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
