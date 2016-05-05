/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

#include "Client_Message.h"

Role_Info::Role_Info(void) {
	reset();
}

Role_Info::~Role_Info() {
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

void Role_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("account: %s, ", account.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("level: %d, ", level);
	printf("exp: %d, ", exp);
	printf("career: %d, ", career);
	printf("gender: %d, ", gender);
	printf("vitality: %d, ", vitality);
	printf("buy_vitality_times: %d, ", buy_vitality_times);
	printf("vip_level: %d, ", vip_level);
	printf("vip_exp: %d, ", vip_exp);
	printf("charge_gold: %d, ", charge_gold);
	printf("skill_point: %d, ", skill_point);
	printf("\n");
}

Property_Detail::Property_Detail(void) {
	reset();
}

Property_Detail::~Property_Detail() {
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

void Property_Detail::print(void) {
	printf("type: %d, ", type);
	printf("value: %d, ", value);
	printf("\n");
}

Item_Info::Item_Info(void) {
	reset();
}

Item_Info::~Item_Info() {
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

void Item_Info::print(void) {
	printf("item_id: %d, ", item_id);
	printf("amount: %d, ", amount);
	printf("level: %d, ", level);
	printf("exp: %d, ", exp);
	printf("\n");
}

Hero_Detail::Hero_Detail(void) {
	reset();
}

Hero_Detail::~Hero_Detail() {
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

void Hero_Detail::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("level: %d, ", level);
	printf("exp: %d, ", exp);
	printf("star: %d, ", star);
	printf("quality: %d, ", quality);
	printf("energy: %d, ", energy);
	uint16_t skill_info_size = (skill_info.size() > 5 ? 5 : skill_info.size());
	printf("skill_info.size: %ld [", skill_info.size());
	for(uint16_t i = 0; i < skill_info_size; ++i) {
		printf("skill_info[i]: %d, ", skill_info[i]);
	}
	printf("], ");
	uint16_t equip_info_size = (equip_info.size() > 5 ? 5 : equip_info.size());
	printf("equip_info.size: %ld [", equip_info.size());
	for(uint16_t i = 0; i < equip_info_size; ++i) {
		equip_info[i].print();
	}
	printf("], ");
	uint16_t property_info_size = (property_info.size() > 5 ? 5 : property_info.size());
	printf("property_info.size: %ld [", property_info.size());
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info[i].print();
	}
	printf("], ");
	printf("\n");
}

Mail_Detail::Mail_Detail(void) {
	reset();
}

Mail_Detail::~Mail_Detail() {
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

void Mail_Detail::print(void) {
	printf("pickup: %d, ", pickup);
	printf("mail_id: %d, ", mail_id);
	printf("send_time: %d, ", send_time);
	printf("sender_type: %d, ", sender_type);
	printf("sender_id: %ld, ", sender_id);
	printf("sender_name: %s, ", sender_name.c_str());
	printf("mail_title: %s, ", mail_title.c_str());
	printf("mail_content: %s, ", mail_content.c_str());
	printf("copper: %d, ", copper);
	printf("gold: %d, ", gold);
	uint16_t item_info_size = (item_info.size() > 5 ? 5 : item_info.size());
	printf("item_info.size: %ld [", item_info.size());
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].print();
	}
	printf("], ");
	printf("\n");
}

Product_Info::Product_Info(void) {
	reset();
}

Product_Info::~Product_Info() {
}

void Product_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(product_id);
	buffer.write_int32(item_id);
	buffer.write_int32(price);
	buffer.write_int32(count);
}

int Product_Info::deserialize(Block_Buffer &buffer) {
	product_id = buffer.read_int32();
	item_id = buffer.read_int32();
	price = buffer.read_int32();
	count = buffer.read_int32();
	return 0;
}

void Product_Info::reset(void) {
	product_id = 0;
	item_id = 0;
	price = 0;
	count = 0;
}

void Product_Info::print(void) {
	printf("product_id: %d, ", product_id);
	printf("item_id: %d, ", item_id);
	printf("price: %d, ", price);
	printf("count: %d, ", count);
	printf("\n");
}

Shop_Detail::Shop_Detail(void) {
	reset();
}

Shop_Detail::~Shop_Detail() {
}

void Shop_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(shop_type);
	buffer.write_int32(refresh_count);
	uint16_t products_size = products.size();
	buffer.write_uint16(products_size);
	for(uint16_t i = 0; i < products_size; ++i) {
		products[i].serialize(buffer);
	}
}

int Shop_Detail::deserialize(Block_Buffer &buffer) {
	shop_type = buffer.read_int32();
	refresh_count = buffer.read_int32();
	uint16_t products_size = buffer.read_uint16();
	Product_Info products_v;
	for(uint16_t i = 0; i < products_size; ++i) {
		products_v.deserialize(buffer);
		products.push_back(products_v);
	}
	return 0;
}

void Shop_Detail::reset(void) {
	shop_type = 0;
	refresh_count = 0;
	products.clear();
}

void Shop_Detail::print(void) {
	printf("shop_type: %d, ", shop_type);
	printf("refresh_count: %d, ", refresh_count);
	uint16_t products_size = (products.size() > 5 ? 5 : products.size());
	printf("products.size: %ld [", products.size());
	for(uint16_t i = 0; i < products_size; ++i) {
		products[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_300000::MSG_300000(void) {
	reset();
}

MSG_300000::~MSG_300000() {
}

void MSG_300000::serialize(Block_Buffer &buffer) const {
}

int MSG_300000::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_300000::reset(void) {
}

void MSG_300000::print(void) {
	printf("\n");
}

MSG_300001::MSG_300001(void) {
	reset();
}

MSG_300001::~MSG_300001() {
}

void MSG_300001::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(player_level);
	buffer.write_int32(player_exp);
}

int MSG_300001::deserialize(Block_Buffer &buffer) {
	player_level = buffer.read_int32();
	player_exp = buffer.read_int32();
	return 0;
}

void MSG_300001::reset(void) {
	player_level = 0;
	player_exp = 0;
}

void MSG_300001::print(void) {
	printf("player_level: %d, ", player_level);
	printf("player_exp: %d, ", player_exp);
	printf("\n");
}

MSG_300002::MSG_300002(void) {
	reset();
}

MSG_300002::~MSG_300002() {
}

void MSG_300002::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(vip_level);
	buffer.write_int32(vip_exp);
}

int MSG_300002::deserialize(Block_Buffer &buffer) {
	vip_level = buffer.read_int32();
	vip_exp = buffer.read_int32();
	return 0;
}

void MSG_300002::reset(void) {
	vip_level = 0;
	vip_exp = 0;
}

void MSG_300002::print(void) {
	printf("vip_level: %d, ", vip_level);
	printf("vip_exp: %d, ", vip_exp);
	printf("\n");
}

MSG_300100::MSG_300100(void) {
	reset();
}

MSG_300100::~MSG_300100() {
}

void MSG_300100::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(copper);
	buffer.write_int32(gold);
}

int MSG_300100::deserialize(Block_Buffer &buffer) {
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	return 0;
}

void MSG_300100::reset(void) {
	copper = 0;
	gold = 0;
}

void MSG_300100::print(void) {
	printf("copper: %d, ", copper);
	printf("gold: %d, ", gold);
	printf("\n");
}

MSG_300101::MSG_300101(void) {
	reset();
}

MSG_300101::~MSG_300101() {
}

void MSG_300101::serialize(Block_Buffer &buffer) const {
	uint16_t item_info_size = item_info.size();
	buffer.write_uint16(item_info_size);
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].serialize(buffer);
	}
}

int MSG_300101::deserialize(Block_Buffer &buffer) {
	uint16_t item_info_size = buffer.read_uint16();
	Item_Info item_info_v;
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info_v.deserialize(buffer);
		item_info.push_back(item_info_v);
	}
	return 0;
}

void MSG_300101::reset(void) {
	item_info.clear();
}

void MSG_300101::print(void) {
	uint16_t item_info_size = (item_info.size() > 5 ? 5 : item_info.size());
	printf("item_info.size: %ld [", item_info.size());
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_300200::MSG_300200(void) {
	reset();
}

MSG_300200::~MSG_300200() {
}

void MSG_300200::serialize(Block_Buffer &buffer) const {
	uint16_t mail_info_size = mail_info.size();
	buffer.write_uint16(mail_info_size);
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info[i].serialize(buffer);
	}
}

int MSG_300200::deserialize(Block_Buffer &buffer) {
	uint16_t mail_info_size = buffer.read_uint16();
	Mail_Detail mail_info_v;
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info_v.deserialize(buffer);
		mail_info.push_back(mail_info_v);
	}
	return 0;
}

void MSG_300200::reset(void) {
	mail_info.clear();
}

void MSG_300200::print(void) {
	uint16_t mail_info_size = (mail_info.size() > 5 ? 5 : mail_info.size());
	printf("mail_info.size: %ld [", mail_info.size());
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_300300::MSG_300300(void) {
	reset();
}

MSG_300300::~MSG_300300() {
}

void MSG_300300::serialize(Block_Buffer &buffer) const {
	uint16_t property_info_size = property_info.size();
	buffer.write_uint16(property_info_size);
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info[i].serialize(buffer);
	}
}

int MSG_300300::deserialize(Block_Buffer &buffer) {
	uint16_t property_info_size = buffer.read_uint16();
	Property_Detail property_info_v;
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info_v.deserialize(buffer);
		property_info.push_back(property_info_v);
	}
	return 0;
}

void MSG_300300::reset(void) {
	property_info.clear();
}

void MSG_300300::print(void) {
	uint16_t property_info_size = (property_info.size() > 5 ? 5 : property_info.size());
	printf("property_info.size: %ld [", property_info.size());
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_120001::MSG_120001(void) {
	reset();
}

MSG_120001::~MSG_120001() {
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

void MSG_120001::print(void) {
	printf("account: %s, ", account.c_str());
	printf("role_id: %ld, ", role_id);
	printf("timestamp: %s, ", timestamp.c_str());
	printf("ip: %s, ", ip.c_str());
	printf("agent_num: %d, ", agent_num);
	printf("server_num: %d, ", server_num);
	printf("\n");
}

MSG_520001::MSG_520001(void) {
	reset();
}

MSG_520001::~MSG_520001() {
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

void MSG_520001::print(void) {
	role_info.print();
	printf("\n");
}

MSG_120002::MSG_120002(void) {
	reset();
}

MSG_120002::~MSG_120002() {
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

void MSG_120002::print(void) {
	printf("account: %s, ", account.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("gender: %d, ", gender);
	printf("agent_num: %d, ", agent_num);
	printf("server_num: %d, ", server_num);
	printf("\n");
}

MSG_520002::MSG_520002(void) {
	reset();
}

MSG_520002::~MSG_520002() {
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

void MSG_520002::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("\n");
}

MSG_120003::MSG_120003(void) {
	reset();
}

MSG_120003::~MSG_120003() {
}

void MSG_120003::serialize(Block_Buffer &buffer) const {
}

int MSG_120003::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120003::reset(void) {
}

void MSG_120003::print(void) {
	printf("\n");
}

MSG_520003::MSG_520003(void) {
	reset();
}

MSG_520003::~MSG_520003() {
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

void MSG_520003::print(void) {
	printf("vitality: %d, ", vitality);
	printf("\n");
}

MSG_120004::MSG_120004(void) {
	reset();
}

MSG_120004::~MSG_120004() {
}

void MSG_120004::serialize(Block_Buffer &buffer) const {
}

int MSG_120004::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120004::reset(void) {
}

void MSG_120004::print(void) {
	printf("\n");
}

MSG_520004::MSG_520004(void) {
	reset();
}

MSG_520004::~MSG_520004() {
}

void MSG_520004::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(copper);
}

int MSG_520004::deserialize(Block_Buffer &buffer) {
	copper = buffer.read_int32();
	return 0;
}

void MSG_520004::reset(void) {
	copper = 0;
}

void MSG_520004::print(void) {
	printf("copper: %d, ", copper);
	printf("\n");
}

MSG_120100::MSG_120100(void) {
	reset();
}

MSG_120100::~MSG_120100() {
}

void MSG_120100::serialize(Block_Buffer &buffer) const {
}

int MSG_120100::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120100::reset(void) {
}

void MSG_120100::print(void) {
	printf("\n");
}

MSG_520100::MSG_520100(void) {
	reset();
}

MSG_520100::~MSG_520100() {
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

void MSG_520100::print(void) {
	uint16_t item_info_size = (item_info.size() > 5 ? 5 : item_info.size());
	printf("item_info.size: %ld [", item_info.size());
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_120101::MSG_120101(void) {
	reset();
}

MSG_120101::~MSG_120101() {
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

void MSG_120101::print(void) {
	item.print();
	printf("\n");
}

MSG_520101::MSG_520101(void) {
	reset();
}

MSG_520101::~MSG_520101() {
}

void MSG_520101::serialize(Block_Buffer &buffer) const {
}

int MSG_520101::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520101::reset(void) {
}

void MSG_520101::print(void) {
	printf("\n");
}

MSG_120102::MSG_120102(void) {
	reset();
}

MSG_120102::~MSG_120102() {
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

void MSG_120102::print(void) {
	item.print();
	printf("\n");
}

MSG_520102::MSG_520102(void) {
	reset();
}

MSG_520102::~MSG_520102() {
}

void MSG_520102::serialize(Block_Buffer &buffer) const {
}

int MSG_520102::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520102::reset(void) {
}

void MSG_520102::print(void) {
	printf("\n");
}

MSG_120200::MSG_120200(void) {
	reset();
}

MSG_120200::~MSG_120200() {
}

void MSG_120200::serialize(Block_Buffer &buffer) const {
}

int MSG_120200::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120200::reset(void) {
}

void MSG_120200::print(void) {
	printf("\n");
}

MSG_520200::MSG_520200(void) {
	reset();
}

MSG_520200::~MSG_520200() {
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

void MSG_520200::print(void) {
	uint16_t mail_info_size = (mail_info.size() > 5 ? 5 : mail_info.size());
	printf("mail_info.size: %ld [", mail_info.size());
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_120201::MSG_120201(void) {
	reset();
}

MSG_120201::~MSG_120201() {
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

void MSG_120201::print(void) {
	printf("mail_id: %d, ", mail_id);
	printf("\n");
}

MSG_520201::MSG_520201(void) {
	reset();
}

MSG_520201::~MSG_520201() {
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

void MSG_520201::print(void) {
	uint16_t mail_id_info_size = (mail_id_info.size() > 5 ? 5 : mail_id_info.size());
	printf("mail_id_info.size: %ld [", mail_id_info.size());
	for(uint16_t i = 0; i < mail_id_info_size; ++i) {
		printf("mail_id_info[i]: %d, ", mail_id_info[i]);
	}
	printf("], ");
	printf("\n");
}

MSG_120202::MSG_120202(void) {
	reset();
}

MSG_120202::~MSG_120202() {
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

void MSG_120202::print(void) {
	printf("mail_id: %d, ", mail_id);
	printf("\n");
}

MSG_520202::MSG_520202(void) {
	reset();
}

MSG_520202::~MSG_520202() {
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

void MSG_520202::print(void) {
	uint16_t mail_id_info_size = (mail_id_info.size() > 5 ? 5 : mail_id_info.size());
	printf("mail_id_info.size: %ld [", mail_id_info.size());
	for(uint16_t i = 0; i < mail_id_info_size; ++i) {
		printf("mail_id_info[i]: %d, ", mail_id_info[i]);
	}
	printf("], ");
	printf("\n");
}

MSG_120203::MSG_120203(void) {
	reset();
}

MSG_120203::~MSG_120203() {
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

void MSG_120203::print(void) {
	printf("receiver_name: %s, ", receiver_name.c_str());
	mail_detail.print();
	printf("\n");
}

MSG_520203::MSG_520203(void) {
	reset();
}

MSG_520203::~MSG_520203() {
}

void MSG_520203::serialize(Block_Buffer &buffer) const {
}

int MSG_520203::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520203::reset(void) {
}

void MSG_520203::print(void) {
	printf("\n");
}

MSG_120300::MSG_120300(void) {
	reset();
}

MSG_120300::~MSG_120300() {
}

void MSG_120300::serialize(Block_Buffer &buffer) const {
}

int MSG_120300::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_120300::reset(void) {
}

void MSG_120300::print(void) {
	printf("\n");
}

MSG_520300::MSG_520300(void) {
	reset();
}

MSG_520300::~MSG_520300() {
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

void MSG_520300::print(void) {
	uint16_t hero_info_size = (hero_info.size() > 5 ? 5 : hero_info.size());
	printf("hero_info.size: %ld [", hero_info.size());
	for(uint16_t i = 0; i < hero_info_size; ++i) {
		hero_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_120301::MSG_120301(void) {
	reset();
}

MSG_120301::~MSG_120301() {
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

void MSG_120301::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("\n");
}

MSG_520301::MSG_520301(void) {
	reset();
}

MSG_520301::~MSG_520301() {
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

void MSG_520301::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("star: %d, ", star);
	printf("\n");
}

MSG_120302::MSG_120302(void) {
	reset();
}

MSG_120302::~MSG_120302() {
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

void MSG_120302::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("\n");
}

MSG_520302::MSG_520302(void) {
	reset();
}

MSG_520302::~MSG_520302() {
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

void MSG_520302::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("quality: %d, ", quality);
	printf("\n");
}

MSG_120303::MSG_120303(void) {
	reset();
}

MSG_120303::~MSG_120303() {
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

void MSG_120303::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("equip_index: %d, ", equip_index);
	uint16_t item_info_size = (item_info.size() > 5 ? 5 : item_info.size());
	printf("item_info.size: %ld [", item_info.size());
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_520303::MSG_520303(void) {
	reset();
}

MSG_520303::~MSG_520303() {
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

void MSG_520303::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("equip_index: %d, ", equip_index);
	printf("equip_level: %d, ", equip_level);
	printf("equip_exp: %d, ", equip_exp);
	printf("\n");
}

MSG_120304::MSG_120304(void) {
	reset();
}

MSG_120304::~MSG_120304() {
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

void MSG_120304::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("equip_index: %d, ", equip_index);
	printf("on: %d, ", on);
	equip_info.print();
	printf("\n");
}

MSG_520304::MSG_520304(void) {
	reset();
}

MSG_520304::~MSG_520304() {
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

void MSG_520304::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("equip_index: %d, ", equip_index);
	printf("on: %d, ", on);
	equip_info.print();
	printf("\n");
}

MSG_120305::MSG_120305(void) {
	reset();
}

MSG_120305::~MSG_120305() {
}

void MSG_120305::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(skill_id);
}

int MSG_120305::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	skill_id = buffer.read_int32();
	return 0;
}

void MSG_120305::reset(void) {
	hero_id = 0;
	skill_id = 0;
}

void MSG_120305::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("skill_id: %d, ", skill_id);
	printf("\n");
}

MSG_520305::MSG_520305(void) {
	reset();
}

MSG_520305::~MSG_520305() {
}

void MSG_520305::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(skill_id);
}

int MSG_520305::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	skill_id = buffer.read_int32();
	return 0;
}

void MSG_520305::reset(void) {
	hero_id = 0;
	skill_id = 0;
}

void MSG_520305::print(void) {
	printf("hero_id: %d, ", hero_id);
	printf("skill_id: %d, ", skill_id);
	printf("\n");
}

MSG_120400::MSG_120400(void) {
	reset();
}

MSG_120400::~MSG_120400() {
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

void MSG_120400::print(void) {
	printf("shop_type: %d, ", shop_type);
	printf("\n");
}

MSG_520400::MSG_520400(void) {
	reset();
}

MSG_520400::~MSG_520400() {
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

void MSG_520400::print(void) {
	shop.print();
	printf("\n");
}

MSG_120401::MSG_120401(void) {
	reset();
}

MSG_120401::~MSG_120401() {
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

void MSG_120401::print(void) {
	printf("shop_type: %d, ", shop_type);
	printf("product_id: %d, ", product_id);
	printf("amount: %d, ", amount);
	printf("\n");
}

MSG_520401::MSG_520401(void) {
	reset();
}

MSG_520401::~MSG_520401() {
}

void MSG_520401::serialize(Block_Buffer &buffer) const {
}

int MSG_520401::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520401::reset(void) {
}

void MSG_520401::print(void) {
	printf("\n");
}

MSG_120402::MSG_120402(void) {
	reset();
}

MSG_120402::~MSG_120402() {
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

void MSG_120402::print(void) {
	printf("shop_type: %d, ", shop_type);
	printf("\n");
}

MSG_520402::MSG_520402(void) {
	reset();
}

MSG_520402::~MSG_520402() {
}

void MSG_520402::serialize(Block_Buffer &buffer) const {
}

int MSG_520402::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_520402::reset(void) {
}

void MSG_520402::print(void) {
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

MSG_110001::MSG_110001(void) {
	reset();
}

MSG_110001::~MSG_110001() {
}

void MSG_110001::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(chat_type);
	buffer.write_string(chat_content);
	buffer.write_string(role_name);
}

int MSG_110001::deserialize(Block_Buffer &buffer) {
	chat_type = buffer.read_int8();
	chat_content = buffer.read_string();
	role_name = buffer.read_string();
	return 0;
}

void MSG_110001::reset(void) {
	chat_type = 0;
	chat_content.clear();
	role_name.clear();
}

void MSG_110001::print(void) {
	printf("chat_type: %d, ", chat_type);
	printf("chat_content: %s, ", chat_content.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("\n");
}

MSG_510001::MSG_510001(void) {
	reset();
}

MSG_510001::~MSG_510001() {
}

void MSG_510001::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(chat_type);
	buffer.write_string(chat_content);
	buffer.write_string(role_name);
	buffer.write_int32(gender);
	buffer.write_int32(career);
	buffer.write_int32(vip_level);
}

int MSG_510001::deserialize(Block_Buffer &buffer) {
	chat_type = buffer.read_int8();
	chat_content = buffer.read_string();
	role_name = buffer.read_string();
	gender = buffer.read_int32();
	career = buffer.read_int32();
	vip_level = buffer.read_int32();
	return 0;
}

void MSG_510001::reset(void) {
	chat_type = 0;
	chat_content.clear();
	role_name.clear();
	gender = 0;
	career = 0;
	vip_level = 0;
}

void MSG_510001::print(void) {
	printf("chat_type: %d, ", chat_type);
	printf("chat_content: %s, ", chat_content.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("gender: %d, ", gender);
	printf("career: %d, ", career);
	printf("vip_level: %d, ", vip_level);
	printf("\n");
}
