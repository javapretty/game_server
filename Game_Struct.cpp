/** 
* struct Game_Struct description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#include "Game_Struct.h"

Account_Info::Account_Info(void) {
	reset();
}

void Account_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
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

Ip_Info::Ip_Info(void) {
	reset();
}

void Ip_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(ip);
	buffer.write_int32(port);
}

int Ip_Info::deserialize(Block_Buffer &buffer) {
	ip = buffer.read_string();
	port = buffer.read_int32();
	return 0;
}

void Ip_Info::reset(void) {
	ip.clear();
	port = 0;
}

Player_DB_Cache::Player_DB_Cache(void) {
	reset();
}

void Player_DB_Cache::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_int32(level);
	buffer.write_int32(gender);
	buffer.write_int32(career);
}

int Player_DB_Cache::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	level = buffer.read_int32();
	gender = buffer.read_int32();
	career = buffer.read_int32();
	return 0;
}

void Player_DB_Cache::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	agent_num = 0;
	server_num = 0;
	level = 0;
	gender = 0;
	career = 0;
}

Login_Player_Info::Login_Player_Info(void) {
	reset();
}

void Login_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(gate_ip);
	buffer.write_int32(gate_port);
	buffer.write_string(session);
	buffer.write_int64(session_tick);
}

int Login_Player_Info::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	gate_ip = buffer.read_string();
	gate_port = buffer.read_int32();
	session = buffer.read_string();
	session_tick = buffer.read_int64();
	return 0;
}

void Login_Player_Info::reset(void) {
	account.clear();
	gate_ip.clear();
	gate_port = 0;
	session.clear();
	session_tick = 0;
}

Master_Player_Info::Master_Player_Info(void) {
	reset();
}

void Master_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(level);
	buffer.write_int32(gender);
	buffer.write_int32(career);
	buffer.write_int32(vip_level);
}

int Master_Player_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	level = buffer.read_int32();
	gender = buffer.read_int32();
	career = buffer.read_int32();
	vip_level = buffer.read_int32();
	return 0;
}

void Master_Player_Info::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	level = 0;
	gender = 0;
	career = 0;
	vip_level = 0;
}

Player_Info::Player_Info(void) {
	reset();
}

void Player_Info::serialize(Block_Buffer &buffer) const {
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
	buffer.write_int32(buy_vitality_times);
	buffer.write_int32(vip_level);
	buffer.write_int32(vip_exp);
	buffer.write_int32(charge_gold);
	buffer.write_int32(skill_point);
	buffer.write_int64(recover_skill_time);
}

int Player_Info::deserialize(Block_Buffer &buffer) {
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
	buy_vitality_times = buffer.read_int32();
	vip_level = buffer.read_int32();
	vip_exp = buffer.read_int32();
	charge_gold = buffer.read_int32();
	skill_point = buffer.read_int32();
	recover_skill_time = buffer.read_int64();
	return 0;
}

void Player_Info::reset(void) {
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
	buy_vitality_times = 0;
	vip_level = 0;
	vip_exp = 0;
	charge_gold = 0;
	skill_point = 0;
	recover_skill_time = 0;
}

Hero_Info::Hero_Info(void) {
	reset();
}

void Hero_Info::serialize(Block_Buffer &buffer) const {

	buffer.write_uint16(hero_map.size());
	for(boost::unordered_map<int32_t,Hero_Detail>::const_iterator it = hero_map.begin();
		it != hero_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Hero_Info::deserialize(Block_Buffer &buffer) {
	uint16_t hero_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < hero_map_size; ++i) {
		Hero_Detail _v;
		_v.deserialize(buffer);
		hero_map[_v.hero_id] =  _v;
	}
	return 0;
}

void Hero_Info::reset(void) {
	hero_map.clear();
}

Bag_Info::Bag_Info(void) {
	reset();
}

void Bag_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(copper);
	buffer.write_int32(gold);

	buffer.write_uint16(item_map.size());
	for(boost::unordered_map<int32_t,Item_Info>::const_iterator it = item_map.begin();
		it != item_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Bag_Info::deserialize(Block_Buffer &buffer) {
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	uint16_t item_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < item_map_size; ++i) {
		Item_Info _v;
		_v.deserialize(buffer);
		item_map[_v.item_id] =  _v;
	}
	return 0;
}

void Bag_Info::reset(void) {
	copper = 0;
	gold = 0;
	item_map.clear();
}

Mail_Info::Mail_Info(void) {
	reset();
}

void Mail_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(total_count);

	buffer.write_uint16(mail_map.size());
	for(std::map<int32_t,Mail_Detail>::const_iterator it = mail_map.begin();
		it != mail_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Mail_Info::deserialize(Block_Buffer &buffer) {
	total_count = buffer.read_int32();
	uint16_t mail_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < mail_map_size; ++i) {
		Mail_Detail _v;
		_v.deserialize(buffer);
		mail_map.insert(std::make_pair(_v.mail_id, _v));
	}
	return 0;
}

void Mail_Info::reset(void) {
	total_count = 0;
	mail_map.clear();
}

Shop_Info::Shop_Info(void) {
	reset();
}

void Shop_Info::serialize(Block_Buffer &buffer) const {

	buffer.write_uint16(shop_detail.size());
	for(boost::unordered_map<int32_t,Shop_Detail>::const_iterator it = shop_detail.begin();
		it != shop_detail.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Shop_Info::deserialize(Block_Buffer &buffer) {
	uint16_t shop_detail_size = buffer.read_uint16();
	for (int16_t i = 0; i < shop_detail_size; ++i) {
		Shop_Detail _v;
		_v.deserialize(buffer);
		shop_detail[_v.shop_type] =  _v;
	}
	return 0;
}

void Shop_Info::reset(void) {
	shop_detail.clear();
}
