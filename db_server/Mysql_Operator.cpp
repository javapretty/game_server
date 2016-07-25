/*
 * Mysql_Operator.cpp
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Mysql_Operator.h"
#include "Server_Config.h"
#include "DB_Manager.h"

Mysql_Operator::Mysql_Operator(void) : mysql_db_conn_(nullptr) { }

Mysql_Operator::~Mysql_Operator(void) {
	MYSQL_DB_MANAGER->RelDBConn(mysql_db_conn_);
}

Mysql_Operator *Mysql_Operator::instance_;

Mysql_Operator *Mysql_Operator::instance(void) {
	if (instance_ == 0)
		instance_ = new Mysql_Operator;
	return instance_;
}

int Mysql_Operator::init(void) {
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

	//连接mysql
	std::string ip(server_misc["mysql_game"]["ip"].asString());
	int port = server_misc["mysql_game"]["port"].asInt();
	std::string user(server_misc["mysql_game"]["user"].asString());
	std::string password(server_misc["mysql_game"]["password"].asString());
	std::string dbname(server_misc["mysql_game"]["dbname"].asString());
	std::string dbpoolname(server_misc["mysql_game"]["dbpoolname"].asString());
	MYSQL_DB_MANAGER->Init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(dbpoolname);

	load_db_cache();
	return 0;
}

int Mysql_Operator::load_db_cache(void) {
	char str_sql[256] = "select * from role";
	sql::ResultSet *resultSet = mysql_db_conn_->ExecuteQuery(str_sql);
	if (resultSet) {
		while(resultSet->next()) {
			Player_DB_Cache db_cache;
			db_cache.role_id = resultSet->getInt64("role_id");
			db_cache.role_name = resultSet->getString("role_name");
			db_cache.account = resultSet->getString("account");
			db_cache.agent_num = resultSet->getInt("agent_num");
			db_cache.server_num = resultSet->getInt("server_num");
			db_cache.level = resultSet->getInt("level");
			DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
			DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
		}
	}
	LOG_INFO("***********load db_cache player count:%d*************", DB_MANAGER->db_cache_account_map().size());
	return 0;
}

int64_t Mysql_Operator::create_player(Game_Player_Info &player_info) {
	return -1;
}
