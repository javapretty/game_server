/*
 * Mongo_Operator.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
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
	DBClientConnection *conn = 0;
	Connection_Map::iterator it = connection_map_.find(thread_key);
	if (it == connection_map_.end()) {
		conn = connection_pool_.pop();

		std::string err_msg;
		std::stringstream host_stream;
		const Json::Value &mongodb_game = SERVER_CONFIG->server_misc()["mongodb_game"];
		if (mongodb_game == Json::Value::null) {
			LOG_FATAL("server_misc config cannot find mongodb_game");
		}
		host_stream << (mongodb_game["ip"].asString());
		if (mongodb_game.isMember("port")) {
			host_stream << ":" << (mongodb_game["port"].asInt());
		}
		conn->connect(host_stream.str().c_str(), err_msg);
		connection_map_[thread_key] = conn;
	} else {
		conn = it->second;
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

	for(DB_Manager::DB_Struct_Id_Map::iterator iter = DB_MANAGER->db_struct_id_map().begin();
			iter != DB_MANAGER->db_struct_id_map().end(); iter++){
		MONGO_CONNECTION.createIndex(iter->second->db_name(), BSON(iter->second->index() << 1));
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
		db_cache.account = obj["account"].valuestrsafe();
		db_cache.role_name = obj["role_name"].valuestrsafe();
		db_cache.agent_num = obj["agent_num"].numberInt();
		db_cache.server_num = obj["server_num"].numberInt();
		db_cache.level = obj["level"].numberInt();
		DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
		DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
	}
	LOG_INFO("***********load db_cache player count:%d*************", DB_MANAGER->db_cache_account_map().size());
	return 0;
}

int64_t Mongo_Operator::create_player(Game_Player_Info &player_info) {
	BSONObj res = MONGO_CONNECTION.findOne("game.role", MONGO_QUERY("role_name" << player_info.role_name));
	if (!res.isEmpty()) {
		LOG_ERROR("create_player role_name = %s existed", player_info.role_name.c_str());
		return -1;
	}

	//从global表查询当前role_id最大值
	BSONObj cmd = fromjson("{findandmodify:'global', query:{type:'role_id'}, update:{$inc:{value:1}}}");
	if (MONGO_CONNECTION.runCommand("game", cmd, res) == false) {
		LOG_ERROR("increase global type='role_id' value failed");
		return -1;
	}

	int order = res.getFieldDotted("value.value").numberLong() + 1;
	int64_t agent = agent_num_ * 10000000000000L;
	int64_t server = server_num_ * 1000000000L;
	int64_t role_id = agent + server + order;

	player_info.role_id = role_id;
	player_info.agent_num = agent_num_;
	player_info.server_num = server_num_;
	int now_sec = Time_Value::gettimeofday().sec();
	MONGO_CONNECTION.update("game.role", MONGO_QUERY("role_id" << ((long long int)role_id)), BSON("$set" <<
			BSON("role_id" << (long long int)role_id
					<< "agent_num" << player_info.agent_num
					<< "server_num" << player_info.server_num
					<< "account" << player_info.account
					<< "role_name" << player_info.role_name
					<< "level" << 1
					<< "gender" << player_info.gender
					<< "career" << 0
					<< "last_sign_in_time" << now_sec
					<< "last_sign_out_time" << now_sec)), true);

	Player_DB_Cache db_cache;
	db_cache.role_id = role_id;
	db_cache.account = player_info.account;
	db_cache.role_name = player_info.role_name;
	db_cache.agent_num = player_info.agent_num;
	db_cache.server_num = player_info.server_num;
	db_cache.level = 1;
	DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
	DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
	LOG_INFO("*************create player,db_cache count:%d***************", DB_MANAGER->db_cache_account_map().size());
	return role_id;
}
