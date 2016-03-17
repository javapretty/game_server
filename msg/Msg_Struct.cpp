/** 
* struct Msg_Struct description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.0]
*
*/

#include "Msg_Struct.h"

Role_Info::Role_Info(void) {
	reset();
}

void Role_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(career);
	buffer.write_int32(gender);
	buffer.write_int32(vitality);
	buffer.write_int32(buy_vitality_times);
	buffer.write_int32(vip_level);
	buffer.write_int32(vip_exp);
	buffer.write_int32(charge_gold);
	buffer.write_int32(skill_point);
}

int Role_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	level = buffer.read_int32();
	exp = buffer.read_int32();
	career = buffer.read_int32();
	gender = buffer.read_int32();
	vitality = buffer.read_int32();
	buy_vitality_times = buffer.read_int32();
	vip_level = buffer.read_int32();
	vip_exp = buffer.read_int32();
	charge_gold = buffer.read_int32();
	skill_point = buffer.read_int32();
	return 0;
}

void Role_Info::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	level = 0;
	exp = 0;
	career = 0;
	gender = 0;
	vitality = 0;
	buy_vitality_times = 0;
	vip_level = 0;
	vip_exp = 0;
	charge_gold = 0;
	skill_point = 0;
}

Property_Detail::Property_Detail(void) {
	reset();
}

void Property_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(type);
	buffer.write_int32(value);
}

int Property_Detail::deserialize(Block_Buffer &buffer) {
	type = buffer.read_int32();
	value = buffer.read_int32();
	return 0;
}

void Property_Detail::reset(void) {
	type = 0;
	value = 0;
}

Item_Info::Item_Info(void) {
	reset();
}

void Item_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(item_id);
	buffer.write_int32(amount);
	buffer.write_int32(level);
	buffer.write_int32(exp);
}

int Item_Info::deserialize(Block_Buffer &buffer) {
	item_id = buffer.read_int32();
	amount = buffer.read_int32();
	level = buffer.read_int32();
	exp = buffer.read_int32();
	return 0;
}

void Item_Info::reset(void) {
	item_id = 0;
	amount = 0;
	level = 0;
	exp = 0;
}

Hero_Detail::Hero_Detail(void) {
	reset();
}

void Hero_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(star);
	buffer.write_int32(quality);
	buffer.write_int32(energy);
	uint16_t skill_info_size = skill_info.size();
	buffer.write_uint16(skill_info_size);
	for(uint16_t i = 0; i < skill_info_size; ++i) {
		buffer.write_int32(skill_info[i]);
	}
	uint16_t equip_info_size = equip_info.size();
	buffer.write_uint16(equip_info_size);
	for(uint16_t i = 0; i < equip_info_size; ++i) {
		equip_info[i].serialize(buffer);
	}
	uint16_t property_info_size = property_info.size();
	buffer.write_uint16(property_info_size);
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info[i].serialize(buffer);
	}
}

int Hero_Detail::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	level = buffer.read_int32();
	exp = buffer.read_int32();
	star = buffer.read_int32();
	quality = buffer.read_int32();
	energy = buffer.read_int32();
	uint16_t skill_info_size = buffer.read_uint16();
	int32_t skill_info_v;
	for(uint16_t i = 0; i < skill_info_size; ++i) {
		skill_info_v = buffer.read_int32();
		skill_info.push_back(skill_info_v);
	}
	uint16_t equip_info_size = buffer.read_uint16();
	Item_Info equip_info_v;
	for(uint16_t i = 0; i < equip_info_size; ++i) {
		equip_info_v.deserialize(buffer);
		equip_info.push_back(equip_info_v);
	}
	uint16_t property_info_size = buffer.read_uint16();
	Property_Detail property_info_v;
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info_v.deserialize(buffer);
		property_info.push_back(property_info_v);
	}
	return 0;
}

void Hero_Detail::reset(void) {
	hero_id = 0;
	level = 0;
	exp = 0;
	star = 0;
	quality = 0;
	energy = 0;
	skill_info.clear();
	equip_info.clear();
	property_info.clear();
}

Mail_Detail::Mail_Detail(void) {
	reset();
}

void Mail_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_bool(pickup);
	buffer.write_int32(mail_id);
	buffer.write_int32(send_time);
	buffer.write_int32(sender_type);
	buffer.write_int64(sender_id);
	buffer.write_string(sender_name);
	buffer.write_string(mail_title);
	buffer.write_string(mail_content);
	buffer.write_int32(copper);
	buffer.write_int32(gold);
	uint16_t item_info_size = item_info.size();
	buffer.write_uint16(item_info_size);
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].serialize(buffer);
	}
}

int Mail_Detail::deserialize(Block_Buffer &buffer) {
	pickup = buffer.read_bool();
	mail_id = buffer.read_int32();
	send_time = buffer.read_int32();
	sender_type = buffer.read_int32();
	sender_id = buffer.read_int64();
	sender_name = buffer.read_string();
	mail_title = buffer.read_string();
	mail_content = buffer.read_string();
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	uint16_t item_info_size = buffer.read_uint16();
	Item_Info item_info_v;
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info_v.deserialize(buffer);
		item_info.push_back(item_info_v);
	}
	return 0;
}

void Mail_Detail::reset(void) {
	pickup = 0;
	mail_id = 0;
	send_time = 0;
	sender_type = 0;
	sender_id = 0;
	sender_name.clear();
	mail_title.clear();
	mail_content.clear();
	copper = 0;
	gold = 0;
	item_info.clear();
}
