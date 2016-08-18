/*
 * Mongo_Operator.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "Mongo_Operator.h"
#include "Server_Config.h"
#include "DB_Manager.h"

Mongo_Operator::Mongo_Operator(void): agent_num_(1), server_num_(1) { }

Mongo_Operator::~Mongo_Operator(void) { }

Mongo_Operator *Mongo_Operator::instance_;

Mongo_Operator *Mongo_Operator::instance(void) {
	if (instance_ == 0)
		instance_ = new Mongo_Operator;
	return instance_;
}

DBClientConnection &Mongo_Operator::connection(void) {
	GUARD(MUTEX, mon, connection_map_lock_);

	const int64_t thread_key = pthread_self();
	DBClientConnection *conn = nullptr;
	Connection_Map::iterator iter = connection_map_.find(thread_key);
	if (iter == connection_map_.end()) {
		conn = connection_pool_.pop();

		//每个线程开启一个mongodb连接，分开处理db请求
		const Json::Value &mongodb_game = SERVER_CONFIG->server_misc()["mongodb_game"];
		if (mongodb_game == Json::Value::null) {
			LOG_FATAL("server_misc error, cannot find mongodb_game!");
		}

		std::string err_msg;
		std::stringstream host_stream;
		host_stream << (mongodb_game["ip"].asString());
		if (mongodb_game.isMember("port")) {
			host_stream << ":" << (mongodb_game["port"].asInt());
		}
		conn->connect(host_stream.str().c_str(), err_msg);
		connection_map_[thread_key] = conn;
	} else {
		conn = iter->second;
	}

  return *conn;
}

int Mongo_Operator::init(void) {
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_misc == Json::Value::null) {
		LOG_FATAL("db init, server_misc null");
		return -1;
	}

	//初始化代理编号，服编号
	agent_num_ = server_misc["agent_num"].asInt();
	server_num_ = server_misc["server_num"].asInt();
	if (agent_num_ < 1) agent_num_ = 1;
	if (server_num_ < 1) server_num_ = 1;

	/// 创建所有索引
	MONGO_CONNECTION.createIndex("game.global", BSON("type" << 1));
	if (MONGO_CONNECTION.count("game.global", MONGO_QUERY("type" << "role_id")) == 0) {
		MONGO_CONNECTION.update("game.global", MONGO_QUERY("type" << "role_id"), BSON("$set" <<
				BSON("type" << "role_id" << "value" << 0)), true);
	}
	if (MONGO_CONNECTION.count("game.global", MONGO_QUERY("type" << "guild_id")) == 0) {
		MONGO_CONNECTION.update("game.global", MONGO_QUERY("type" << "guild_id"), BSON("$set" <<
				BSON("type" << "guild_id" << "value" << 0)), true);
	}

	//role
	MONGO_CONNECTION.createIndex("game.role", BSON("role_id" << 1));
	MONGO_CONNECTION.createIndex("game.role", BSON("role_name" << 1));
	MONGO_CONNECTION.createIndex("game.role", BSON("account" << 1));
	MONGO_CONNECTION.createIndex("game.role", BSON("account" << 1 << "level" << -1));

	for(Struct_Id_Map::iterator iter = DB_MANAGER->db_struct_id_map().begin();
			iter != DB_MANAGER->db_struct_id_map().end(); iter++){
		MONGO_CONNECTION.createIndex(iter->second->table_name(), BSON(iter->second->index_name() << 1));
	}

	load_db_cache();
	return 0;
}

int Mongo_Operator::load_db_cache(void) {
	unsigned long long count = MONGO_CONNECTION.count("game.role");
	std::vector<BSONObj> iter_record;
	iter_record.reserve(count);
	MONGO_CONNECTION.findN(iter_record, "game.role", Query(), count);
	for (std::vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj obj = *iter;

		Player_DB_Cache db_cache;
		db_cache.role_id = obj["role_id"].numberLong();
		db_cache.role_name = obj["role_name"].valuestrsafe();
		db_cache.account = obj["account"].valuestrsafe();
		db_cache.agent_num = obj["agent_num"].numberInt();
		db_cache.server_num = obj["server_num"].numberInt();
		DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
		DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
	}
	LOG_INFO("***********load db_cache player count:%d*************", DB_MANAGER->db_cache_account_map().size());
	return 0;
}

int64_t Mongo_Operator::generate_id(std::string type) {
	BSONObj result;
	char str_json[128] = {0};
	sprintf(str_json, "{findandmodify:'global', query:{type:'%s'}, update:{$inc:{value:1}}}", type.c_str());
	if (MONGO_CONNECTION.runCommand("game", fromjson(str_json), result) == false) {
		LOG_ERROR("findandmodify global type='%s' value failed", type.c_str());
		return -1;
	}

	int order = result.getFieldDotted("value.value").numberLong() + 1;
	int64_t agent = agent_num_ * 10000000000000L;
	int64_t server = server_num_ * 1000000000L;
	int64_t id = agent + server + order;
	return id;
}

int64_t Mongo_Operator::create_player(Create_Role_Info &role_info) {
	BSONObj result = MONGO_CONNECTION.findOne("game.role", MONGO_QUERY("account" << role_info.account << "role_name" << role_info.role_name));
	if (!result.isEmpty()) {
		LOG_ERROR("create_player account = %s role_name = %s existed", role_info.account.c_str(), role_info.role_name.c_str());
		return -1;
	}

	int64_t role_id = generate_id("role_id");
	if (role_id < 0) {
		LOG_ERROR("create_player generate_id error, role_id:%ld, account = %s role_name = %s", role_id, role_info.account.c_str(), role_info.role_name.c_str());
		return -1;
	}

	int now_sec = Time_Value::gettimeofday().sec();
	MONGO_CONNECTION.update("game.role", MONGO_QUERY("role_id" << ((long long int)role_id)), BSON("$set" <<
			BSON("role_id" << (long long int)role_id
					<< "role_name" << role_info.role_name
					<< "account" << role_info.account
					<< "agent_num" << agent_num_
					<< "server_num" << server_num_
					<< "level" << 1
					<< "gender" << role_info.gender
					<< "career" << role_info.career
					<< "create_time" << now_sec
					<< "login_time" << now_sec)), true);

	Player_DB_Cache db_cache;
	db_cache.role_id = role_id;
	db_cache.role_name = role_info.role_name;
	db_cache.account = role_info.account;
	db_cache.agent_num = agent_num_;
	db_cache.server_num = server_num_;
	DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
	DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
	LOG_INFO("***************create role, role_id:%ld, db_cache count:%d***************", role_id, DB_MANAGER->db_cache_account_map().size());
	return role_id;
}

int64_t Mongo_Operator::create_guild(Create_Guild_Info &guild_info) {
	BSONObj res = MONGO_CONNECTION.findOne("game.guild", MONGO_QUERY("guild_name" << guild_info.guild_name));
	if (!res.isEmpty()) {
		LOG_ERROR("create_guild guild_name = %s existed", guild_info.guild_name.c_str());
		return -1;
	}

	int64_t guild_id = generate_id("guild_id");
	if (guild_id < 0) {
		LOG_ERROR("create_guild generate_id error, create_guild:%ld, guild_name = %s", guild_id, guild_info.guild_name.c_str());
		return -1;
	}

	Struct_Name_Map::iterator iter = DB_MANAGER->db_struct_name_map().find("Guild_Info");
	if(iter == DB_MANAGER->db_struct_name_map().end()){
		LOG_ERROR("Can not find the struct_name Guild_Info");
		return -1;
	}
	Block_Buffer buf;
	iter->second->create_data(guild_id, buf);
	LOG_INFO("***************create guild,guild_id:%ld***************", guild_id);
	return guild_id;
}
