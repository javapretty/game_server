/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.4]
*/

#include "Game_Struct.h"

Rank_Member::Rank_Member(void) {
	reset();
}

Rank_Member::~Rank_Member() {
}

void Rank_Member::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(role_name);
	buffer.write_int32(value);
}

int Rank_Member::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	role_name = buffer.read_string();
	value = buffer.read_int32();
	return 0;
}

void Rank_Member::reset(void) {
	role_id = 0;
	role_name.clear();
	value = 0;
}

void Rank_Member::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("role_name: %s, ", role_name.c_str());
	printf("value: %d, ", value);
	printf("\n");
}

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

Property_Detail::Property_Detail(void) {
	reset();
}

Property_Detail::~Property_Detail() {
}

void Property_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(type);
	buffer.write_int32(name);
}

int Property_Detail::deserialize(Block_Buffer &buffer) {
	type = buffer.read_int32();
	name = buffer.read_int32();
	return 0;
}

void Property_Detail::reset(void) {
	type = 0;
	name = 0;
}

void Property_Detail::print(void) {
	printf("type: %d, ", type);
	printf("name: %d, ", name);
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

Guild_Member_Detail::Guild_Member_Detail(void) {
	reset();
}

Guild_Member_Detail::~Guild_Member_Detail() {
}

void Guild_Member_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(role_name);
	buffer.write_int32(level);
	buffer.write_int32(career);
}

int Guild_Member_Detail::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	role_name = buffer.read_string();
	level = buffer.read_int32();
	career = buffer.read_int32();
	return 0;
}

void Guild_Member_Detail::reset(void) {
	role_id = 0;
	role_name.clear();
	level = 0;
	career = 0;
}

void Guild_Member_Detail::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("role_name: %s, ", role_name.c_str());
	printf("level: %d, ", level);
	printf("career: %d, ", career);
	printf("\n");
}

Game_Player_Info::Game_Player_Info(void) {
	reset();
}

Game_Player_Info::~Game_Player_Info() {
}

void Game_Player_Info::serialize(Block_Buffer &buffer) const {
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
	buffer.write_int32(guild_id);
	buffer.write_string(guild_name);
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
	buy_vitality_times = buffer.read_int32();
	vip_level = buffer.read_int32();
	vip_exp = buffer.read_int32();
	charge_gold = buffer.read_int32();
	guild_id = buffer.read_int32();
	guild_name = buffer.read_string();
	return 0;
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
	buy_vitality_times = 0;
	vip_level = 0;
	vip_exp = 0;
	charge_gold = 0;
	guild_id = 0;
	guild_name.clear();
}

void Game_Player_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("account: %s, ", account.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("client_ip: %s, ", client_ip.c_str());
	printf("agent_num: %d, ", agent_num);
	printf("server_num: %d, ", server_num);
	printf("level: %d, ", level);
	printf("exp: %d, ", exp);
	printf("gender: %d, ", gender);
	printf("career: %d, ", career);
	printf("create_time: %d, ", create_time);
	printf("last_sign_in_time: %d, ", last_sign_in_time);
	printf("last_sign_out_time: %d, ", last_sign_out_time);
	printf("vitality: %d, ", vitality);
	printf("buy_vitality_times: %d, ", buy_vitality_times);
	printf("vip_level: %d, ", vip_level);
	printf("vip_exp: %d, ", vip_exp);
	printf("charge_gold: %d, ", charge_gold);
	printf("guild_id: %d, ", guild_id);
	printf("guild_name: %s, ", guild_name.c_str());
	printf("\n");
}

Hero_Info::Hero_Info(void) {
	reset();
}

Hero_Info::~Hero_Info() {
}

void Hero_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);

	buffer.write_uint16(hero_map.size());
	for(boost::unordered_map<int32_t,Hero_Detail>::const_iterator it = hero_map.begin();
		it != hero_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Hero_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	uint16_t hero_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < hero_map_size; ++i) {
		Hero_Detail _v;
		_v.deserialize(buffer);
		hero_map[_v.hero_id] =  _v;
	}
	return 0;
}

void Hero_Info::reset(void) {
	role_id = 0;
	hero_map.clear();
}

void Hero_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("hero_map.size: %ld {}, ", hero_map.size());
	printf("\n");
}

Bag_Info::Bag_Info(void) {
	reset();
}

Bag_Info::~Bag_Info() {
}

void Bag_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int32(copper);
	buffer.write_int32(gold);

	buffer.write_uint16(item_map.size());
	for(boost::unordered_map<int32_t,Item_Info>::const_iterator it = item_map.begin();
		it != item_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Bag_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
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
	role_id = 0;
	copper = 0;
	gold = 0;
	item_map.clear();
}

void Bag_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("copper: %d, ", copper);
	printf("gold: %d, ", gold);
	printf("item_map.size: %ld {}, ", item_map.size());
	printf("\n");
}

Mail_Info::Mail_Info(void) {
	reset();
}

Mail_Info::~Mail_Info() {
}

void Mail_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int32(total_count);

	buffer.write_uint16(mail_map.size());
	for(boost::unordered_map<int32_t,Mail_Detail>::const_iterator it = mail_map.begin();
		it != mail_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Mail_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	total_count = buffer.read_int32();
	uint16_t mail_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < mail_map_size; ++i) {
		Mail_Detail _v;
		_v.deserialize(buffer);
		mail_map[_v.mail_id] =  _v;
	}
	return 0;
}

void Mail_Info::reset(void) {
	role_id = 0;
	total_count = 0;
	mail_map.clear();
}

void Mail_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("total_count: %d, ", total_count);
	printf("mail_map.size: %ld {}, ", mail_map.size());
	printf("\n");
}

Shop_Info::Shop_Info(void) {
	reset();
}

Shop_Info::~Shop_Info() {
}

void Shop_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);

	buffer.write_uint16(shop_map.size());
	for(boost::unordered_map<int32_t,Shop_Detail>::const_iterator it = shop_map.begin();
		it != shop_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Shop_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	uint16_t shop_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < shop_map_size; ++i) {
		Shop_Detail _v;
		_v.deserialize(buffer);
		shop_map[_v.shop_type] =  _v;
	}
	return 0;
}

void Shop_Info::reset(void) {
	role_id = 0;
	shop_map.clear();
}

void Shop_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("shop_map.size: %ld {}, ", shop_map.size());
	printf("\n");
}

Rank_Info::Rank_Info(void) {
	reset();
}

Rank_Info::~Rank_Info() {
}

void Rank_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(rank_type);
	buffer.write_int32(min_value);
	buffer.write_int64(min_role_id);

	buffer.write_uint16(member_map.size());
	for(boost::unordered_map<int64_t,Rank_Member>::const_iterator it = member_map.begin();
		it != member_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Rank_Info::deserialize(Block_Buffer &buffer) {
	rank_type = buffer.read_int64();
	min_value = buffer.read_int32();
	min_role_id = buffer.read_int64();
	uint16_t member_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < member_map_size; ++i) {
		Rank_Member _v;
		_v.deserialize(buffer);
		member_map[_v.role_id] =  _v;
	}
	return 0;
}

void Rank_Info::reset(void) {
	rank_type = 0;
	min_value = 0;
	min_role_id = 0;
	member_map.clear();
}

void Rank_Info::print(void) {
	printf("rank_type: %ld, ", rank_type);
	printf("min_value: %d, ", min_value);
	printf("min_role_id: %ld, ", min_role_id);
	printf("member_map.size: %ld {}, ", member_map.size());
	printf("\n");
}

Guild_Info::Guild_Info(void) {
	reset();
}

Guild_Info::~Guild_Info() {
}

void Guild_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(guild_id);
	buffer.write_bool(change);
	buffer.write_string(guild_name);
	buffer.write_int64(chief_id);
	uint16_t applicant_list_size = applicant_list.size();
	buffer.write_uint16(applicant_list_size);
	for(uint16_t i = 0; i < applicant_list_size; ++i) {
		applicant_list[i].serialize(buffer);
	}
	uint16_t member_list_size = member_list.size();
	buffer.write_uint16(member_list_size);
	for(uint16_t i = 0; i < member_list_size; ++i) {
		member_list[i].serialize(buffer);
	}
}

int Guild_Info::deserialize(Block_Buffer &buffer) {
	guild_id = buffer.read_int64();
	change = buffer.read_bool();
	guild_name = buffer.read_string();
	chief_id = buffer.read_int64();
	uint16_t applicant_list_size = buffer.read_uint16();
	Guild_Member_Detail applicant_list_v;
	for(uint16_t i = 0; i < applicant_list_size; ++i) {
		applicant_list_v.deserialize(buffer);
		applicant_list.push_back(applicant_list_v);
	}
	uint16_t member_list_size = buffer.read_uint16();
	Guild_Member_Detail member_list_v;
	for(uint16_t i = 0; i < member_list_size; ++i) {
		member_list_v.deserialize(buffer);
		member_list.push_back(member_list_v);
	}
	return 0;
}

void Guild_Info::reset(void) {
	guild_id = 0;
	change = 0;
	guild_name.clear();
	chief_id = 0;
	applicant_list.clear();
	member_list.clear();
}

void Guild_Info::print(void) {
	printf("guild_id: %ld, ", guild_id);
	printf("change: %d, ", change);
	printf("guild_name: %s, ", guild_name.c_str());
	printf("chief_id: %ld, ", chief_id);
	uint16_t applicant_list_size = (applicant_list.size() > 5 ? 5 : applicant_list.size());
	printf("applicant_list.size: %ld [", applicant_list.size());
	for(uint16_t i = 0; i < applicant_list_size; ++i) {
		applicant_list[i].print();
	}
	printf("], ");
	uint16_t member_list_size = (member_list.size() > 5 ? 5 : member_list.size());
	printf("member_list.size: %ld [", member_list.size());
	for(uint16_t i = 0; i < member_list_size; ++i) {
		member_list[i].print();
	}
	printf("], ");
	printf("\n");
}

Offline_Info::Offline_Info(void) {
	reset();
}

Offline_Info::~Offline_Info() {
}

void Offline_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int64(guild_id);
	buffer.write_string(guild_name);
	buffer.write_int16(flag);
}

int Offline_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	guild_id = buffer.read_int64();
	guild_name = buffer.read_string();
	flag = buffer.read_int16();
	return 0;
}

void Offline_Info::reset(void) {
	role_id = 0;
	guild_id = 0;
	guild_name.clear();
	flag = 0;
}

void Offline_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("guild_id: %ld, ", guild_id);
	printf("guild_name: %s, ", guild_name.c_str());
	printf("flag: %d, ", flag);
	printf("\n");
}

Player_DB_Cache::Player_DB_Cache(void) {
	reset();
}

Player_DB_Cache::~Player_DB_Cache() {
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

void Player_DB_Cache::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("account: %s, ", account.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("agent_num: %d, ", agent_num);
	printf("server_num: %d, ", server_num);
	printf("level: %d, ", level);
	printf("gender: %d, ", gender);
	printf("career: %d, ", career);
	printf("\n");
}

Master_Player_Info::Master_Player_Info(void) {
	reset();
}

Master_Player_Info::~Master_Player_Info() {
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

void Master_Player_Info::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("account: %s, ", account.c_str());
	printf("role_name: %s, ", role_name.c_str());
	printf("level: %d, ", level);
	printf("gender: %d, ", gender);
	printf("career: %d, ", career);
	printf("vip_level: %d, ", vip_level);
	printf("\n");
}
