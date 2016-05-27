/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

#include "Server_Message.h"

Account_Info::Account_Info(void) {
	reset();
}

Account_Info::~Account_Info() {
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

void Account_Info::print(void) {
	printf("account: %s, ", account.c_str());
	printf("agent_num: %d, ", agent_num);
	printf("server_num: %d, ", server_num);
	printf("\n");
}

Ip_Info::Ip_Info(void) {
	reset();
}

Ip_Info::~Ip_Info() {
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

void Ip_Info::print(void) {
	printf("ip: %s, ", ip.c_str());
	printf("port: %d, ", port);
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

Login_Player_Info::Login_Player_Info(void) {
	reset();
}

Login_Player_Info::~Login_Player_Info() {
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

void Login_Player_Info::print(void) {
	printf("account: %s, ", account.c_str());
	printf("gate_ip: %s, ", gate_ip.c_str());
	printf("gate_port: %d, ", gate_port);
	printf("session: %s, ", session.c_str());
	printf("session_tick: %ld, ", session_tick);
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
	buffer.write_int32(skill_point);
	buffer.write_int64(recover_skill_time);
	buffer.write_int32(exchange_count);
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
	skill_point = buffer.read_int32();
	recover_skill_time = buffer.read_int64();
	exchange_count = buffer.read_int32();
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
	skill_point = 0;
	recover_skill_time = 0;
	exchange_count = 0;
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
	printf("skill_point: %d, ", skill_point);
	printf("recover_skill_time: %ld, ", recover_skill_time);
	printf("exchange_count: %d, ", exchange_count);
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

void Hero_Info::print(void) {
	printf("hero_map.size: %ld {}, ", hero_map.size());
	printf("\n");
}

Bag_Info::Bag_Info(void) {
	reset();
}

Bag_Info::~Bag_Info() {
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

void Bag_Info::print(void) {
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

void Mail_Info::print(void) {
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

void Shop_Info::print(void) {
	printf("shop_detail.size: %ld {}, ", shop_detail.size());
	printf("\n");
}

Player_Data::Player_Data(void) {
	reset();
}

Player_Data::~Player_Data() {
}

void Player_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(status);
	uint16_t change_module_size = change_module.size();
	buffer.write_uint16(change_module_size);
	for(uint16_t i = 0; i < change_module_size; ++i) {
		buffer.write_int32(change_module[i]);
	}
	player_info.serialize(buffer);
	hero_info.serialize(buffer);
	bag_info.serialize(buffer);
	mail_info.serialize(buffer);
	shop_info.serialize(buffer);
}

int Player_Data::deserialize(Block_Buffer &buffer) {
	status = buffer.read_int8();
	uint16_t change_module_size = buffer.read_uint16();
	int32_t change_module_v;
	for(uint16_t i = 0; i < change_module_size; ++i) {
		change_module_v = buffer.read_int32();
		change_module.push_back(change_module_v);
	}
	player_info.deserialize(buffer);
	hero_info.deserialize(buffer);
	bag_info.deserialize(buffer);
	mail_info.deserialize(buffer);
	shop_info.deserialize(buffer);
	return 0;
}

void Player_Data::reset(void) {
	status = 0;
	change_module.clear();
	player_info.reset();
	hero_info.reset();
	bag_info.reset();
	mail_info.reset();
	shop_info.reset();
}

void Player_Data::print(void) {
	printf("status: %d, ", status);
	uint16_t change_module_size = (change_module.size() > 5 ? 5 : change_module.size());
	printf("change_module.size: %ld [", change_module.size());
	for(uint16_t i = 0; i < change_module_size; ++i) {
		printf("change_module[i]: %d, ", change_module[i]);
	}
	printf("], ");
	player_info.print();
	hero_info.print();
	bag_info.print();
	mail_info.print();
	shop_info.print();
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

Guild_Detail::Guild_Detail(void) {
	reset();
}

Guild_Detail::~Guild_Detail() {
}

void Guild_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_bool(change);
	buffer.write_int64(guild_id);
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

int Guild_Detail::deserialize(Block_Buffer &buffer) {
	change = buffer.read_bool();
	guild_id = buffer.read_int64();
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

void Guild_Detail::reset(void) {
	change = 0;
	guild_id = 0;
	guild_name.clear();
	chief_id = 0;
	applicant_list.clear();
	member_list.clear();
}

void Guild_Detail::print(void) {
	printf("change: %d, ", change);
	printf("guild_id: %ld, ", guild_id);
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

Guild_Info::Guild_Info(void) {
	reset();
}

Guild_Info::~Guild_Info() {
}

void Guild_Info::serialize(Block_Buffer &buffer) const {

	buffer.write_uint16(guild_map.size());
	for(boost::unordered_map<int64_t,Guild_Detail>::const_iterator it = guild_map.begin();
		it != guild_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Guild_Info::deserialize(Block_Buffer &buffer) {
	uint16_t guild_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < guild_map_size; ++i) {
		Guild_Detail _v;
		_v.deserialize(buffer);
		guild_map[_v.guild_id] =  _v;
	}
	return 0;
}

void Guild_Info::reset(void) {
	guild_map.clear();
}

void Guild_Info::print(void) {
	printf("guild_map.size: %ld {}, ", guild_map.size());
	printf("\n");
}

Offline_Detail::Offline_Detail(void) {
	reset();
}

Offline_Detail::~Offline_Detail() {
}

void Offline_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int64(guild_id);
	buffer.write_string(guild_name);
	buffer.write_int16(flag);
}

int Offline_Detail::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	guild_id = buffer.read_int64();
	guild_name = buffer.read_string();
	flag = buffer.read_int16();
	return 0;
}

void Offline_Detail::reset(void) {
	role_id = 0;
	guild_id = 0;
	guild_name.clear();
	flag = 0;
}

void Offline_Detail::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("guild_id: %ld, ", guild_id);
	printf("guild_name: %s, ", guild_name.c_str());
	printf("flag: %d, ", flag);
	printf("\n");
}

Offline_Info::Offline_Info(void) {
	reset();
}

Offline_Info::~Offline_Info() {
}

void Offline_Info::serialize(Block_Buffer &buffer) const {

	buffer.write_uint16(offline_map.size());
	for(boost::unordered_map<int64_t,Offline_Detail>::const_iterator it = offline_map.begin();
		it != offline_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Offline_Info::deserialize(Block_Buffer &buffer) {
	uint16_t offline_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < offline_map_size; ++i) {
		Offline_Detail _v;
		_v.deserialize(buffer);
		offline_map[_v.role_id] =  _v;
	}
	return 0;
}

void Offline_Info::reset(void) {
	offline_map.clear();
}

void Offline_Info::print(void) {
	printf("offline_map.size: %ld {}, ", offline_map.size());
	printf("\n");
}

Rank_Info::Rank_Info(void) {
	reset();
}

Rank_Info::~Rank_Info() {
}

void Rank_Info::serialize(Block_Buffer &buffer) const {

	buffer.write_uint16(rank_map.size());
	for(std::map<int32_t,Rank_List>::const_iterator it = rank_map.begin();
		it != rank_map.end(); ++it) {
		it->second.serialize(buffer);
	}
}

int Rank_Info::deserialize(Block_Buffer &buffer) {
	uint16_t rank_map_size = buffer.read_uint16();
	for (int16_t i = 0; i < rank_map_size; ++i) {
		Rank_List _v;
		_v.deserialize(buffer);
		rank_map.insert(std::make_pair(_v.rank_type, _v));
	}
	return 0;
}

void Rank_Info::reset(void) {
	rank_map.clear();
}

void Rank_Info::print(void) {
	printf("rank_map.size: %ld {}, ", rank_map.size());
	printf("\n");
}

MSG_150000::MSG_150000(void) {
	reset();
}

MSG_150000::~MSG_150000() {
}

void MSG_150000::serialize(Block_Buffer &buffer) const {
}

int MSG_150000::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_150000::reset(void) {
}

void MSG_150000::print(void) {
	printf("\n");
}

MSG_550000::MSG_550000(void) {
	reset();
}

MSG_550000::~MSG_550000() {
}

void MSG_550000::serialize(Block_Buffer &buffer) const {
	uint16_t db_cache_vec_size = db_cache_vec.size();
	buffer.write_uint16(db_cache_vec_size);
	for(uint16_t i = 0; i < db_cache_vec_size; ++i) {
		db_cache_vec[i].serialize(buffer);
	}
}

int MSG_550000::deserialize(Block_Buffer &buffer) {
	uint16_t db_cache_vec_size = buffer.read_uint16();
	Player_DB_Cache db_cache_vec_v;
	for(uint16_t i = 0; i < db_cache_vec_size; ++i) {
		db_cache_vec_v.deserialize(buffer);
		db_cache_vec.push_back(db_cache_vec_v);
	}
	return 0;
}

void MSG_550000::reset(void) {
	db_cache_vec.clear();
}

void MSG_550000::print(void) {
	uint16_t db_cache_vec_size = (db_cache_vec.size() > 5 ? 5 : db_cache_vec.size());
	printf("db_cache_vec.size: %ld [", db_cache_vec.size());
	for(uint16_t i = 0; i < db_cache_vec_size; ++i) {
		db_cache_vec[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_150001::MSG_150001(void) {
	reset();
}

MSG_150001::~MSG_150001() {
}

void MSG_150001::serialize(Block_Buffer &buffer) const {
	account_info.serialize(buffer);
}

int MSG_150001::deserialize(Block_Buffer &buffer) {
	account_info.deserialize(buffer);
	return 0;
}

void MSG_150001::reset(void) {
	account_info.reset();
}

void MSG_150001::print(void) {
	account_info.print();
	printf("\n");
}

MSG_550001::MSG_550001(void) {
	reset();
}

MSG_550001::~MSG_550001() {
}

void MSG_550001::serialize(Block_Buffer &buffer) const {
	player_data.serialize(buffer);
}

int MSG_550001::deserialize(Block_Buffer &buffer) {
	player_data.deserialize(buffer);
	return 0;
}

void MSG_550001::reset(void) {
	player_data.reset();
}

void MSG_550001::print(void) {
	player_data.print();
	printf("\n");
}

MSG_150002::MSG_150002(void) {
	reset();
}

MSG_150002::~MSG_150002() {
}

void MSG_150002::serialize(Block_Buffer &buffer) const {
	player_info.serialize(buffer);
}

int MSG_150002::deserialize(Block_Buffer &buffer) {
	player_info.deserialize(buffer);
	return 0;
}

void MSG_150002::reset(void) {
	player_info.reset();
}

void MSG_150002::print(void) {
	player_info.print();
	printf("\n");
}

MSG_550002::MSG_550002(void) {
	reset();
}

MSG_550002::~MSG_550002() {
}

void MSG_550002::serialize(Block_Buffer &buffer) const {
	player_data.serialize(buffer);
}

int MSG_550002::deserialize(Block_Buffer &buffer) {
	player_data.deserialize(buffer);
	return 0;
}

void MSG_550002::reset(void) {
	player_data.reset();
}

void MSG_550002::print(void) {
	player_data.print();
	printf("\n");
}

MSG_150003::MSG_150003(void) {
	reset();
}

MSG_150003::~MSG_150003() {
}

void MSG_150003::serialize(Block_Buffer &buffer) const {
	player_data.serialize(buffer);
}

int MSG_150003::deserialize(Block_Buffer &buffer) {
	player_data.deserialize(buffer);
	return 0;
}

void MSG_150003::reset(void) {
	player_data.reset();
}

void MSG_150003::print(void) {
	player_data.print();
	printf("\n");
}

MSG_550003::MSG_550003(void) {
	reset();
}

MSG_550003::~MSG_550003() {
}

void MSG_550003::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
}

int MSG_550003::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	return 0;
}

void MSG_550003::reset(void) {
	role_id = 0;
}

void MSG_550003::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("\n");
}

MSG_150004::MSG_150004(void) {
	reset();
}

MSG_150004::~MSG_150004() {
}

void MSG_150004::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	mail_detail.serialize(buffer);
}

int MSG_150004::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	mail_detail.deserialize(buffer);
	return 0;
}

void MSG_150004::reset(void) {
	role_id = 0;
	mail_detail.reset();
}

void MSG_150004::print(void) {
	printf("role_id: %ld, ", role_id);
	mail_detail.print();
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

MSG_140100::MSG_140100(void) {
	reset();
}

MSG_140100::~MSG_140100() {
}

void MSG_140100::serialize(Block_Buffer &buffer) const {
}

int MSG_140100::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_140100::reset(void) {
}

void MSG_140100::print(void) {
	printf("\n");
}

MSG_140200::MSG_140200(void) {
	reset();
}

MSG_140200::~MSG_140200() {
}

void MSG_140200::serialize(Block_Buffer &buffer) const {
	player_info.serialize(buffer);
}

int MSG_140200::deserialize(Block_Buffer &buffer) {
	player_info.deserialize(buffer);
	return 0;
}

void MSG_140200::reset(void) {
	player_info.reset();
}

void MSG_140200::print(void) {
	player_info.print();
	printf("\n");
}

MSG_180000::MSG_180000(void) {
	reset();
}

MSG_180000::~MSG_180000() {
}

void MSG_180000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(id);
	buffer.write_string(name);
}

int MSG_180000::deserialize(Block_Buffer &buffer) {
	id = buffer.read_int32();
	name = buffer.read_string();
	return 0;
}

void MSG_180000::reset(void) {
	id = 0;
	name.clear();
}

void MSG_180000::print(void) {
	printf("id: %d, ", id);
	printf("name: %s, ", name.c_str());
	printf("\n");
}

MSG_180001::MSG_180001(void) {
	reset();
}

MSG_180001::~MSG_180001() {
}

void MSG_180001::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(role_name);
	buffer.write_string(account);
	buffer.write_uint16(level);
	buffer.write_string(client_ip);
	buffer.write_uint32(login_time);
	buffer.write_uint32(logout_time);
	buffer.write_uint32(online_time);
}

int MSG_180001::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	role_name = buffer.read_string();
	account = buffer.read_string();
	level = buffer.read_uint16();
	client_ip = buffer.read_string();
	login_time = buffer.read_uint32();
	logout_time = buffer.read_uint32();
	online_time = buffer.read_uint32();
	return 0;
}

void MSG_180001::reset(void) {
	role_id = 0;
	role_name.clear();
	account.clear();
	level = 0;
	client_ip.clear();
	login_time = 0;
	logout_time = 0;
	online_time = 0;
}

void MSG_180001::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("role_name: %s, ", role_name.c_str());
	printf("account: %s, ", account.c_str());
	printf("level: %d, ", level);
	printf("client_ip: %s, ", client_ip.c_str());
	printf("login_time: %d, ", login_time);
	printf("logout_time: %d, ", logout_time);
	printf("online_time: %d, ", online_time);
	printf("\n");
}

MSG_160000::MSG_160000(void) {
	reset();
}

MSG_160000::~MSG_160000() {
}

void MSG_160000::serialize(Block_Buffer &buffer) const {
	player_info.serialize(buffer);
}

int MSG_160000::deserialize(Block_Buffer &buffer) {
	player_info.deserialize(buffer);
	return 0;
}

void MSG_160000::reset(void) {
	player_info.reset();
}

void MSG_160000::print(void) {
	player_info.print();
	printf("\n");
}

MSG_160001::MSG_160001(void) {
	reset();
}

MSG_160001::~MSG_160001() {
}

void MSG_160001::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
}

int MSG_160001::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	return 0;
}

void MSG_160001::reset(void) {
	role_id = 0;
}

void MSG_160001::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("\n");
}

MSG_160100::MSG_160100(void) {
	reset();
}

MSG_160100::~MSG_160100() {
}

void MSG_160100::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(guild_id);
	buffer.write_string(guild_name);
}

int MSG_160100::deserialize(Block_Buffer &buffer) {
	guild_id = buffer.read_int64();
	guild_name = buffer.read_string();
	return 0;
}

void MSG_160100::reset(void) {
	guild_id = 0;
	guild_name.clear();
}

void MSG_160100::print(void) {
	printf("guild_id: %ld, ", guild_id);
	printf("guild_name: %s, ", guild_name.c_str());
	printf("\n");
}

MSG_165000::MSG_165000(void) {
	reset();
}

MSG_165000::~MSG_165000() {
}

void MSG_165000::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(level);
}

int MSG_165000::deserialize(Block_Buffer &buffer) {
	level = buffer.read_int32();
	return 0;
}

void MSG_165000::reset(void) {
	level = 0;
}

void MSG_165000::print(void) {
	printf("level: %d, ", level);
	printf("\n");
}

MSG_150101::MSG_150101(void) {
	reset();
}

MSG_150101::~MSG_150101() {
}

void MSG_150101::serialize(Block_Buffer &buffer) const {
}

int MSG_150101::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_150101::reset(void) {
}

void MSG_150101::print(void) {
	printf("\n");
}

MSG_550101::MSG_550101(void) {
	reset();
}

MSG_550101::~MSG_550101() {
}

void MSG_550101::serialize(Block_Buffer &buffer) const {
	guild_info.serialize(buffer);
	offline_info.serialize(buffer);
	rank_info.serialize(buffer);
}

int MSG_550101::deserialize(Block_Buffer &buffer) {
	guild_info.deserialize(buffer);
	offline_info.deserialize(buffer);
	rank_info.deserialize(buffer);
	return 0;
}

void MSG_550101::reset(void) {
	guild_info.reset();
	offline_info.reset();
	rank_info.reset();
}

void MSG_550101::print(void) {
	guild_info.print();
	offline_info.print();
	rank_info.print();
	printf("\n");
}

MSG_150102::MSG_150102(void) {
	reset();
}

MSG_150102::~MSG_150102() {
}

void MSG_150102::serialize(Block_Buffer &buffer) const {
	guild_info.serialize(buffer);
}

int MSG_150102::deserialize(Block_Buffer &buffer) {
	guild_info.deserialize(buffer);
	return 0;
}

void MSG_150102::reset(void) {
	guild_info.reset();
}

void MSG_150102::print(void) {
	guild_info.print();
	printf("\n");
}

MSG_550102::MSG_550102(void) {
	reset();
}

MSG_550102::~MSG_550102() {
}

void MSG_550102::serialize(Block_Buffer &buffer) const {
}

int MSG_550102::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550102::reset(void) {
}

void MSG_550102::print(void) {
	printf("\n");
}

MSG_150103::MSG_150103(void) {
	reset();
}

MSG_150103::~MSG_150103() {
}

void MSG_150103::serialize(Block_Buffer &buffer) const {
	uint16_t guild_list_size = guild_list.size();
	buffer.write_uint16(guild_list_size);
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		buffer.write_int64(guild_list[i]);
	}
}

int MSG_150103::deserialize(Block_Buffer &buffer) {
	uint16_t guild_list_size = buffer.read_uint16();
	int64_t guild_list_v;
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		guild_list_v = buffer.read_int64();
		guild_list.push_back(guild_list_v);
	}
	return 0;
}

void MSG_150103::reset(void) {
	guild_list.clear();
}

void MSG_150103::print(void) {
	uint16_t guild_list_size = (guild_list.size() > 5 ? 5 : guild_list.size());
	printf("guild_list.size: %ld [", guild_list.size());
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		printf("guild_list[i]: %ld, ", guild_list[i]);
	}
	printf("], ");
	printf("\n");
}

MSG_550103::MSG_550103(void) {
	reset();
}

MSG_550103::~MSG_550103() {
}

void MSG_550103::serialize(Block_Buffer &buffer) const {
}

int MSG_550103::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550103::reset(void) {
}

void MSG_550103::print(void) {
	printf("\n");
}

MSG_150104::MSG_150104(void) {
	reset();
}

MSG_150104::~MSG_150104() {
}

void MSG_150104::serialize(Block_Buffer &buffer) const {
	offline_info.serialize(buffer);
}

int MSG_150104::deserialize(Block_Buffer &buffer) {
	offline_info.deserialize(buffer);
	return 0;
}

void MSG_150104::reset(void) {
	offline_info.reset();
}

void MSG_150104::print(void) {
	offline_info.print();
	printf("\n");
}

MSG_550104::MSG_550104(void) {
	reset();
}

MSG_550104::~MSG_550104() {
}

void MSG_550104::serialize(Block_Buffer &buffer) const {
}

int MSG_550104::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550104::reset(void) {
}

void MSG_550104::print(void) {
	printf("\n");
}

MSG_150105::MSG_150105(void) {
	reset();
}

MSG_150105::~MSG_150105() {
}

void MSG_150105::serialize(Block_Buffer &buffer) const {
	uint16_t offline_list_size = offline_list.size();
	buffer.write_uint16(offline_list_size);
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		buffer.write_int64(offline_list[i]);
	}
}

int MSG_150105::deserialize(Block_Buffer &buffer) {
	uint16_t offline_list_size = buffer.read_uint16();
	int64_t offline_list_v;
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list_v = buffer.read_int64();
		offline_list.push_back(offline_list_v);
	}
	return 0;
}

void MSG_150105::reset(void) {
	offline_list.clear();
}

void MSG_150105::print(void) {
	uint16_t offline_list_size = (offline_list.size() > 5 ? 5 : offline_list.size());
	printf("offline_list.size: %ld [", offline_list.size());
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		printf("offline_list[i]: %ld, ", offline_list[i]);
	}
	printf("], ");
	printf("\n");
}

MSG_550105::MSG_550105(void) {
	reset();
}

MSG_550105::~MSG_550105() {
}

void MSG_550105::serialize(Block_Buffer &buffer) const {
}

int MSG_550105::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550105::reset(void) {
}

void MSG_550105::print(void) {
	printf("\n");
}

MSG_150106::MSG_150106(void) {
	reset();
}

MSG_150106::~MSG_150106() {
}

void MSG_150106::serialize(Block_Buffer &buffer) const {
	rank_info.serialize(buffer);
}

int MSG_150106::deserialize(Block_Buffer &buffer) {
	rank_info.deserialize(buffer);
	return 0;
}

void MSG_150106::reset(void) {
	rank_info.reset();
}

void MSG_150106::print(void) {
	rank_info.print();
	printf("\n");
}

MSG_550106::MSG_550106(void) {
	reset();
}

MSG_550106::~MSG_550106() {
}

void MSG_550106::serialize(Block_Buffer &buffer) const {
}

int MSG_550106::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550106::reset(void) {
}

void MSG_550106::print(void) {
	printf("\n");
}
