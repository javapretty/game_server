#include "Msg_Struct.h"


Role_Info::Role_Info(void){
	reset();
}

void Role_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(career);
	buffer.write_int32(gender);
	buffer.write_int32(vitality);
	buffer.write_int32(vip);
	buffer.write_int32(charge_gold);
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
}

int Role_Info::deserialize(Block_Buffer &buffer) {
	level = buffer.read_int32();
	exp = buffer.read_int32();
	career = buffer.read_int32();
	gender = buffer.read_int32();
	vitality = buffer.read_int32();
	vip = buffer.read_int32();
	charge_gold = buffer.read_int32();
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	return 0;
}

void Role_Info::reset(){
	level = 0;
	exp = 0;
	career = 0;
	gender = 0;
	vitality = 0;
	vip = 0;
	charge_gold = 0;
	role_id = 0;
	account.clear();
	role_name.clear();
}

Property_Detail::Property_Detail(void){
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

void Property_Detail::reset(){
	type = 0;
	value = 0;
}

Item_Info::Item_Info(void){
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

void Item_Info::reset(){
	item_id = 0;
	amount = 0;
	level = 0;
	exp = 0;
}

Hero_Detail::Hero_Detail(void){
	reset();
}

void Hero_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(star);
	buffer.write_int32(quality);
	uint16_t __equip_info_vec_size = equip_info.size();
	buffer.write_uint16(__equip_info_vec_size);
	for(uint16_t i = 0; i < __equip_info_vec_size; ++i) {
		equip_info[i].serialize(buffer);
	}

	uint16_t __property_info_vec_size = property_info.size();
	buffer.write_uint16(__property_info_vec_size);
	for(uint16_t i = 0; i < __property_info_vec_size; ++i) {
		property_info[i].serialize(buffer);
	}

}

int Hero_Detail::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	level = buffer.read_int32();
	exp = buffer.read_int32();
	star = buffer.read_int32();
	quality = buffer.read_int32();
	uint16_t __equip_info_vec_size = buffer.read_uint16();
	Item_Info v0;
	for(uint16_t i = 0; i < __equip_info_vec_size; ++i) {
		if(v0.deserialize(buffer))
			return -1;
		equip_info.push_back(v0);
	}

	uint16_t __property_info_vec_size = buffer.read_uint16();
	Property_Detail v2;
	for(uint16_t i = 0; i < __property_info_vec_size; ++i) {
		if(v2.deserialize(buffer))
			return -1;
		property_info.push_back(v2);
	}

	return 0;
}

void Hero_Detail::reset(){
	hero_id = 0;
	level = 0;
	exp = 0;
	star = 0;
	quality = 0;
	equip_info.clear();
	property_info.clear();
}

Mail_Detail::Mail_Detail(void){
	reset();
}

void Mail_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(mail_id);
	buffer.write_int32(send_time);
	buffer.write_int32(sender_type);
	buffer.write_int32(copper);
	buffer.write_int32(gold);
	buffer.write_int64(sender_id);
	buffer.write_bool(pickup);
	buffer.write_string(sender_name);
	buffer.write_string(mail_title);
	buffer.write_string(mail_content);
	uint16_t __item_info_vec_size = item_info.size();
	buffer.write_uint16(__item_info_vec_size);
	for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
		item_info[i].serialize(buffer);
	}

}

int Mail_Detail::deserialize(Block_Buffer &buffer) {
	mail_id = buffer.read_int32();
	send_time = buffer.read_int32();
	sender_type = buffer.read_int32();
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	sender_id = buffer.read_int64();
	pickup = buffer.read_bool();
	sender_name = buffer.read_string();
	mail_title = buffer.read_string();
	mail_content = buffer.read_string();
	uint16_t __item_info_vec_size = buffer.read_uint16();
	Item_Info v0;
	for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
		if(v0.deserialize(buffer))
			return -1;
		item_info.push_back(v0);
	}

	return 0;
}

void Mail_Detail::reset(){
	mail_id = 0;
	send_time = 0;
	sender_type = 0;
	copper = 0;
	gold = 0;
	sender_id = 0;
	pickup = false;
	sender_name.clear();
	mail_title.clear();
	mail_content.clear();
	item_info.clear();
}
