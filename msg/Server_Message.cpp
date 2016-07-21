/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.4]
*/

#include "Server_Message.h"

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
	buffer.write_string(account);
}

int MSG_150001::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	return 0;
}

void MSG_150001::reset(void) {
	account.clear();
}

void MSG_150001::print(void) {
	printf("account: %s, ", account.c_str());
	printf("\n");
}

MSG_550001::MSG_550001(void) {
	reset();
}

MSG_550001::~MSG_550001() {
}

void MSG_550001::serialize(Block_Buffer &buffer) const {
}

int MSG_550001::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550001::reset(void) {
}

void MSG_550001::print(void) {
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
}

int MSG_550002::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_550002::reset(void) {
}

void MSG_550002::print(void) {
	printf("\n");
}

MSG_150003::MSG_150003(void) {
	reset();
}

MSG_150003::~MSG_150003() {
}

void MSG_150003::serialize(Block_Buffer &buffer) const {
}

int MSG_150003::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_150003::reset(void) {
}

void MSG_150003::print(void) {
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

MSG_150008::MSG_150008(void) {
	reset();
}

MSG_150008::~MSG_150008() {
}

void MSG_150008::serialize(Block_Buffer &buffer) const {
	buffer.write_string(table_name);
	uint16_t delete_list_size = delete_list.size();
	buffer.write_uint16(delete_list_size);
	for(uint16_t i = 0; i < delete_list_size; ++i) {
		buffer.write_int64(delete_list[i]);
	}
}

int MSG_150008::deserialize(Block_Buffer &buffer) {
	table_name = buffer.read_string();
	uint16_t delete_list_size = buffer.read_uint16();
	int64_t delete_list_v;
	for(uint16_t i = 0; i < delete_list_size; ++i) {
		delete_list_v = buffer.read_int64();
		delete_list.push_back(delete_list_v);
	}
	return 0;
}

void MSG_150008::reset(void) {
	table_name.clear();
	delete_list.clear();
}

void MSG_150008::print(void) {
	printf("table_name: %s, ", table_name.c_str());
	uint16_t delete_list_size = (delete_list.size() > 5 ? 5 : delete_list.size());
	printf("delete_list.size: %ld [", delete_list.size());
	for(uint16_t i = 0; i < delete_list_size; ++i) {
		printf("delete_list[i]: %ld, ", delete_list[i]);
	}
	printf("], ");
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

MSG_150102::MSG_150102(void) {
	reset();
}

MSG_150102::~MSG_150102() {
}

void MSG_150102::serialize(Block_Buffer &buffer) const {
	buffer.write_string(table_name);
	uint16_t delete_list_size = delete_list.size();
	buffer.write_uint16(delete_list_size);
	for(uint16_t i = 0; i < delete_list_size; ++i) {
		buffer.write_int64(delete_list[i]);
	}
}

int MSG_150102::deserialize(Block_Buffer &buffer) {
	table_name = buffer.read_string();
	uint16_t delete_list_size = buffer.read_uint16();
	int64_t delete_list_v;
	for(uint16_t i = 0; i < delete_list_size; ++i) {
		delete_list_v = buffer.read_int64();
		delete_list.push_back(delete_list_v);
	}
	return 0;
}

void MSG_150102::reset(void) {
	table_name.clear();
	delete_list.clear();
}

void MSG_150102::print(void) {
	printf("table_name: %s, ", table_name.c_str());
	uint16_t delete_list_size = (delete_list.size() > 5 ? 5 : delete_list.size());
	printf("delete_list.size: %ld [", delete_list.size());
	for(uint16_t i = 0; i < delete_list_size; ++i) {
		printf("delete_list[i]: %ld, ", delete_list[i]);
	}
	printf("], ");
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
		guild_list[i].serialize(buffer);
	}
}

int MSG_150103::deserialize(Block_Buffer &buffer) {
	uint16_t guild_list_size = buffer.read_uint16();
	Guild_Info guild_list_v;
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		guild_list_v.deserialize(buffer);
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
		guild_list[i].print();
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
	uint16_t guild_list_size = guild_list.size();
	buffer.write_uint16(guild_list_size);
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		guild_list[i].serialize(buffer);
	}
}

int MSG_550103::deserialize(Block_Buffer &buffer) {
	uint16_t guild_list_size = buffer.read_uint16();
	Guild_Info guild_list_v;
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		guild_list_v.deserialize(buffer);
		guild_list.push_back(guild_list_v);
	}
	return 0;
}

void MSG_550103::reset(void) {
	guild_list.clear();
}

void MSG_550103::print(void) {
	uint16_t guild_list_size = (guild_list.size() > 5 ? 5 : guild_list.size());
	printf("guild_list.size: %ld [", guild_list.size());
	for(uint16_t i = 0; i < guild_list_size; ++i) {
		guild_list[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_150104::MSG_150104(void) {
	reset();
}

MSG_150104::~MSG_150104() {
}

void MSG_150104::serialize(Block_Buffer &buffer) const {
	uint16_t offline_list_size = offline_list.size();
	buffer.write_uint16(offline_list_size);
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list[i].serialize(buffer);
	}
}

int MSG_150104::deserialize(Block_Buffer &buffer) {
	uint16_t offline_list_size = buffer.read_uint16();
	Offline_Info offline_list_v;
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list_v.deserialize(buffer);
		offline_list.push_back(offline_list_v);
	}
	return 0;
}

void MSG_150104::reset(void) {
	offline_list.clear();
}

void MSG_150104::print(void) {
	uint16_t offline_list_size = (offline_list.size() > 5 ? 5 : offline_list.size());
	printf("offline_list.size: %ld [", offline_list.size());
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_550104::MSG_550104(void) {
	reset();
}

MSG_550104::~MSG_550104() {
}

void MSG_550104::serialize(Block_Buffer &buffer) const {
	uint16_t offline_list_size = offline_list.size();
	buffer.write_uint16(offline_list_size);
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list[i].serialize(buffer);
	}
}

int MSG_550104::deserialize(Block_Buffer &buffer) {
	uint16_t offline_list_size = buffer.read_uint16();
	Offline_Info offline_list_v;
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list_v.deserialize(buffer);
		offline_list.push_back(offline_list_v);
	}
	return 0;
}

void MSG_550104::reset(void) {
	offline_list.clear();
}

void MSG_550104::print(void) {
	uint16_t offline_list_size = (offline_list.size() > 5 ? 5 : offline_list.size());
	printf("offline_list.size: %ld [", offline_list.size());
	for(uint16_t i = 0; i < offline_list_size; ++i) {
		offline_list[i].print();
	}
	printf("], ");
	printf("\n");
}

MSG_150105::MSG_150105(void) {
	reset();
}

MSG_150105::~MSG_150105() {
}

void MSG_150105::serialize(Block_Buffer &buffer) const {
	uint16_t rank_list_size = rank_list.size();
	buffer.write_uint16(rank_list_size);
	for(uint16_t i = 0; i < rank_list_size; ++i) {
		rank_list[i].serialize(buffer);
	}
}

int MSG_150105::deserialize(Block_Buffer &buffer) {
	uint16_t rank_list_size = buffer.read_uint16();
	Rank_Info rank_list_v;
	for(uint16_t i = 0; i < rank_list_size; ++i) {
		rank_list_v.deserialize(buffer);
		rank_list.push_back(rank_list_v);
	}
	return 0;
}

void MSG_150105::reset(void) {
	rank_list.clear();
}

void MSG_150105::print(void) {
	uint16_t rank_list_size = (rank_list.size() > 5 ? 5 : rank_list.size());
	printf("rank_list.size: %ld [", rank_list.size());
	for(uint16_t i = 0; i < rank_list_size; ++i) {
		rank_list[i].print();
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
	uint16_t rank_list_size = rank_list.size();
	buffer.write_uint16(rank_list_size);
	for(uint16_t i = 0; i < rank_list_size; ++i) {
		rank_list[i].serialize(buffer);
	}
}

int MSG_550105::deserialize(Block_Buffer &buffer) {
	uint16_t rank_list_size = buffer.read_uint16();
	Rank_Info rank_list_v;
	for(uint16_t i = 0; i < rank_list_size; ++i) {
		rank_list_v.deserialize(buffer);
		rank_list.push_back(rank_list_v);
	}
	return 0;
}

void MSG_550105::reset(void) {
	rank_list.clear();
}

void MSG_550105::print(void) {
	uint16_t rank_list_size = (rank_list.size() > 5 ? 5 : rank_list.size());
	printf("rank_list.size: %ld [", rank_list.size());
	for(uint16_t i = 0; i < rank_list_size; ++i) {
		rank_list[i].print();
	}
	printf("], ");
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
	buffer.write_int32(login_time);
	buffer.write_int32(logout_time);
}

int MSG_180001::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	role_name = buffer.read_string();
	account = buffer.read_string();
	level = buffer.read_uint16();
	client_ip = buffer.read_string();
	login_time = buffer.read_int32();
	logout_time = buffer.read_int32();
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
}

void MSG_180001::print(void) {
	printf("role_id: %ld, ", role_id);
	printf("role_name: %s, ", role_name.c_str());
	printf("account: %s, ", account.c_str());
	printf("level: %d, ", level);
	printf("client_ip: %s, ", client_ip.c_str());
	printf("login_time: %d, ", login_time);
	printf("logout_time: %d, ", logout_time);
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
