/** 
* struct Client_Game_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.0]
*
*/

#include "Client_Game_Message.h"

MSG_120001::MSG_120001(void) {
	reset();
}

void MSG_120001::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_int64(role_id);
	buffer.write_string(timestamp);
	buffer.write_string(ip);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
}

int MSG_120001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	role_id = buffer.read_int64();
	timestamp = buffer.read_string();
	ip = buffer.read_string();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	return 0;
}

void MSG_120001::reset(void) {
	account.clear();
	role_id = 0;
	timestamp.clear();
	ip.clear();
	agent_num = 0;
	server_num = 0;
}

MSG_520001::MSG_520001(void) {
	reset();
}

void MSG_520001::serialize(Block_Buffer &buffer) const {
	role_info.serialize(buffer);
}

int MSG_520001::deserialize(Block_Buffer &buffer) {
	role_info.deserialize(buffer);
	return 0;
}

void MSG_520001::reset(void) {
	role_info.reset();
}

MSG_120002::MSG_120002(void) {
	reset();
}

void MSG_120002::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_uint8(gender);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
}

int MSG_120002::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	role_name = buffer.read_string();
	gender = buffer.read_uint8();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	return 0;
}

void MSG_120002::reset(void) {
	account.clear();
	role_name.clear();
	gender = 0;
	agent_num = 0;
	server_num = 0;
}

MSG_520002::MSG_520002(void) {
	reset();
}

void MSG_520002::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
}

int MSG_520002::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	return 0;
}

void MSG_520002::reset(void) {
	role_id = 0;
}

MSG_120003::MSG_120003(void) {
	reset();
}

void MSG_120003::serialize(Block_Buffer &buffer) const {
}

int MSG_120003::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120003::reset(void) {
}

MSG_520003::MSG_520003(void) {
	reset();
}

void MSG_520003::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(vitality);
}

int MSG_520003::deserialize(Block_Buffer &buffer) {
	vitality = buffer.read_int32();
	return 0;
}

void MSG_520003::reset(void) {
	vitality = 0;
}

MSG_120100::MSG_120100(void) {
	reset();
}

void MSG_120100::serialize(Block_Buffer &buffer) const {
}

int MSG_120100::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120100::reset(void) {
}

MSG_520100::MSG_520100(void) {
	reset();
}

void MSG_520100::serialize(Block_Buffer &buffer) const {
	uint16_t item_info_size = item_info.size();
	buffer.write_uint16(item_info_size);
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].serialize(buffer);
	}
}

int MSG_520100::deserialize(Block_Buffer &buffer) {
	uint16_t item_info_size = buffer.read_uint16();
	Item_Info item_info_v;
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info_v.deserialize(buffer);
		item_info.push_back(item_info_v);
	}
	return 0;
}

void MSG_520100::reset(void) {
	item_info.clear();
}

MSG_120101::MSG_120101(void) {
	reset();
}

void MSG_120101::serialize(Block_Buffer &buffer) const {
	item.serialize(buffer);
}

int MSG_120101::deserialize(Block_Buffer &buffer) {
	item.deserialize(buffer);
	return 0;
}

void MSG_120101::reset(void) {
	item.reset();
}

MSG_520101::MSG_520101(void) {
	reset();
}

void MSG_520101::serialize(Block_Buffer &buffer) const {
}

int MSG_520101::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520101::reset(void) {
}

MSG_120102::MSG_120102(void) {
	reset();
}

void MSG_120102::serialize(Block_Buffer &buffer) const {
	item.serialize(buffer);
}

int MSG_120102::deserialize(Block_Buffer &buffer) {
	item.deserialize(buffer);
	return 0;
}

void MSG_120102::reset(void) {
	item.reset();
}

MSG_520102::MSG_520102(void) {
	reset();
}

void MSG_520102::serialize(Block_Buffer &buffer) const {
}

int MSG_520102::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520102::reset(void) {
}

MSG_120200::MSG_120200(void) {
	reset();
}

void MSG_120200::serialize(Block_Buffer &buffer) const {
}

int MSG_120200::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120200::reset(void) {
}

MSG_520200::MSG_520200(void) {
	reset();
}

void MSG_520200::serialize(Block_Buffer &buffer) const {
	uint16_t mail_info_size = mail_info.size();
	buffer.write_uint16(mail_info_size);
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info[i].serialize(buffer);
	}
}

int MSG_520200::deserialize(Block_Buffer &buffer) {
	uint16_t mail_info_size = buffer.read_uint16();
	Mail_Detail mail_info_v;
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info_v.deserialize(buffer);
		mail_info.push_back(mail_info_v);
	}
	return 0;
}

void MSG_520200::reset(void) {
	mail_info.clear();
}

MSG_120201::MSG_120201(void) {
	reset();
}

void MSG_120201::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(mail_id);
}

int MSG_120201::deserialize(Block_Buffer &buffer) {
	mail_id = buffer.read_int32();
	return 0;
}

void MSG_120201::reset(void) {
	mail_id = 0;
}

MSG_520201::MSG_520201(void) {
	reset();
}

void MSG_520201::serialize(Block_Buffer &buffer) const {
	uint16_t mail_id_info_size = mail_id_info.size();
	buffer.write_uint16(mail_id_info_size);
	for(uint16_t i = 0; i < mail_id_info_size; ++i) {
		buffer.write_int32(mail_id_info[i]);
	}
}

int MSG_520201::deserialize(Block_Buffer &buffer) {
	uint16_t mail_id_info_size = buffer.read_uint16();
	int32_t mail_id_info_v;
	for(uint16_t i = 0; i < mail_id_info_size; ++i) {
		mail_id_info_v = buffer.read_int32();
		mail_id_info.push_back(mail_id_info_v);
	}
	return 0;
}

void MSG_520201::reset(void) {
	mail_id_info.clear();
}

MSG_120202::MSG_120202(void) {
	reset();
}

void MSG_120202::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(mail_id);
}

int MSG_120202::deserialize(Block_Buffer &buffer) {
	mail_id = buffer.read_int32();
	return 0;
}

void MSG_120202::reset(void) {
	mail_id = 0;
}

MSG_520202::MSG_520202(void) {
	reset();
}

void MSG_520202::serialize(Block_Buffer &buffer) const {
	uint16_t mail_id_info_size = mail_id_info.size();
	buffer.write_uint16(mail_id_info_size);
	for(uint16_t i = 0; i < mail_id_info_size; ++i) {
		buffer.write_int32(mail_id_info[i]);
	}
}

int MSG_520202::deserialize(Block_Buffer &buffer) {
	uint16_t mail_id_info_size = buffer.read_uint16();
	int32_t mail_id_info_v;
	for(uint16_t i = 0; i < mail_id_info_size; ++i) {
		mail_id_info_v = buffer.read_int32();
		mail_id_info.push_back(mail_id_info_v);
	}
	return 0;
}

void MSG_520202::reset(void) {
	mail_id_info.clear();
}

MSG_120203::MSG_120203(void) {
	reset();
}

void MSG_120203::serialize(Block_Buffer &buffer) const {
	buffer.write_string(receiver_name);
	mail_detail.serialize(buffer);
}

int MSG_120203::deserialize(Block_Buffer &buffer) {
	receiver_name = buffer.read_string();
	mail_detail.deserialize(buffer);
	return 0;
}

void MSG_120203::reset(void) {
	receiver_name.clear();
	mail_detail.reset();
}

MSG_520203::MSG_520203(void) {
	reset();
}

void MSG_520203::serialize(Block_Buffer &buffer) const {
}

int MSG_520203::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520203::reset(void) {
}

MSG_120300::MSG_120300(void) {
	reset();
}

void MSG_120300::serialize(Block_Buffer &buffer) const {
}

int MSG_120300::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120300::reset(void) {
}

MSG_520300::MSG_520300(void) {
	reset();
}

void MSG_520300::serialize(Block_Buffer &buffer) const {
	uint16_t hero_info_size = hero_info.size();
	buffer.write_uint16(hero_info_size);
	for(uint16_t i = 0; i < hero_info_size; ++i) {
		hero_info[i].serialize(buffer);
	}
}

int MSG_520300::deserialize(Block_Buffer &buffer) {
	uint16_t hero_info_size = buffer.read_uint16();
	Hero_Detail hero_info_v;
	for(uint16_t i = 0; i < hero_info_size; ++i) {
		hero_info_v.deserialize(buffer);
		hero_info.push_back(hero_info_v);
	}
	return 0;
}

void MSG_520300::reset(void) {
	hero_info.clear();
}

MSG_120301::MSG_120301(void) {
	reset();
}

void MSG_120301::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
}

int MSG_120301::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	return 0;
}

void MSG_120301::reset(void) {
	hero_id = 0;
}

MSG_520301::MSG_520301(void) {
	reset();
}

void MSG_520301::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(star);
}

int MSG_520301::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	star = buffer.read_int32();
	return 0;
}

void MSG_520301::reset(void) {
	hero_id = 0;
	star = 0;
}

MSG_120302::MSG_120302(void) {
	reset();
}

void MSG_120302::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
}

int MSG_120302::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	return 0;
}

void MSG_120302::reset(void) {
	hero_id = 0;
}

MSG_520302::MSG_520302(void) {
	reset();
}

void MSG_520302::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(quality);
}

int MSG_520302::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	quality = buffer.read_int32();
	return 0;
}

void MSG_520302::reset(void) {
	hero_id = 0;
	quality = 0;
}

MSG_120303::MSG_120303(void) {
	reset();
}

void MSG_120303::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(equip_index);
	uint16_t item_info_size = item_info.size();
	buffer.write_uint16(item_info_size);
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].serialize(buffer);
	}
}

int MSG_120303::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	equip_index = buffer.read_int32();
	uint16_t item_info_size = buffer.read_uint16();
	Item_Info item_info_v;
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info_v.deserialize(buffer);
		item_info.push_back(item_info_v);
	}
	return 0;
}

void MSG_120303::reset(void) {
	hero_id = 0;
	equip_index = 0;
	item_info.clear();
}

MSG_520303::MSG_520303(void) {
	reset();
}

void MSG_520303::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(equip_index);
	buffer.write_int32(equip_level);
	buffer.write_int32(equip_exp);
}

int MSG_520303::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	equip_index = buffer.read_int32();
	equip_level = buffer.read_int32();
	equip_exp = buffer.read_int32();
	return 0;
}

void MSG_520303::reset(void) {
	hero_id = 0;
	equip_index = 0;
	equip_level = 0;
	equip_exp = 0;
}

MSG_120304::MSG_120304(void) {
	reset();
}

void MSG_120304::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(equip_index);
	buffer.write_bool(on);
	equip_info.serialize(buffer);
}

int MSG_120304::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	equip_index = buffer.read_int32();
	on = buffer.read_bool();
	equip_info.deserialize(buffer);
	return 0;
}

void MSG_120304::reset(void) {
	hero_id = 0;
	equip_index = 0;
	on = 0;
	equip_info.reset();
}

MSG_520304::MSG_520304(void) {
	reset();
}

void MSG_520304::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(equip_index);
	buffer.write_bool(on);
	equip_info.serialize(buffer);
}

int MSG_520304::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	equip_index = buffer.read_int32();
	on = buffer.read_bool();
	equip_info.deserialize(buffer);
	return 0;
}

void MSG_520304::reset(void) {
	hero_id = 0;
	equip_index = 0;
	on = 0;
	equip_info.reset();
}

MSG_120400::MSG_120400(void) {
	reset();
}

void MSG_120400::serialize(Block_Buffer &buffer) const {
	buffer.write_uint32(shop_type);
}

int MSG_120400::deserialize(Block_Buffer &buffer) {
	shop_type = buffer.read_uint32();
	return 0;
}

void MSG_120400::reset(void) {
	shop_type = 0;
}

MSG_520400::MSG_520400(void) {
	reset();
}

void MSG_520400::serialize(Block_Buffer &buffer) const {
	shop.serialize(buffer);
}

int MSG_520400::deserialize(Block_Buffer &buffer) {
	shop.deserialize(buffer);
	return 0;
}

void MSG_520400::reset(void) {
	shop.reset();
}

MSG_120401::MSG_120401(void) {
	reset();
}

void MSG_120401::serialize(Block_Buffer &buffer) const {
	buffer.write_uint32(shop_type);
	buffer.write_uint32(product_id);
	buffer.write_uint32(amount);
}

int MSG_120401::deserialize(Block_Buffer &buffer) {
	shop_type = buffer.read_uint32();
	product_id = buffer.read_uint32();
	amount = buffer.read_uint32();
	return 0;
}

void MSG_120401::reset(void) {
	shop_type = 0;
	product_id = 0;
	amount = 0;
}

MSG_520401::MSG_520401(void) {
	reset();
}

void MSG_520401::serialize(Block_Buffer &buffer) const {
}

int MSG_520401::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520401::reset(void) {
}

MSG_120402::MSG_120402(void) {
	reset();
}

void MSG_120402::serialize(Block_Buffer &buffer) const {
	buffer.write_uint32(shop_type);
}

int MSG_120402::deserialize(Block_Buffer &buffer) {
	shop_type = buffer.read_uint32();
	return 0;
}

void MSG_120402::reset(void) {
	shop_type = 0;
}

MSG_520402::MSG_520402(void) {
	reset();
}

void MSG_520402::serialize(Block_Buffer &buffer) const {
}

int MSG_520402::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520402::reset(void) {
}
