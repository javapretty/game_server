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

Mysql_Operator::Mysql_Operator(void) : mysql_db_conn_(nullptr), agent_num_(0), server_num_(0) { }

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

	sql::ResultSet *resultSet = mysql_db_conn_->ExecuteQuery("select * from global where type='role_id'");
	if (!resultSet) {
		mysql_db_conn_->Execute("insert into global(type, value) values ('role_id', 0)");
	}
	resultSet = mysql_db_conn_->ExecuteQuery("select * from global where type='guild_id'");
	if (!resultSet) {
		mysql_db_conn_->Execute("insert into global(type, value) values ('guild_id', 0)");
	}

	load_db_cache();
	return 0;
}

int Mysql_Operator::load_db_cache(void) {
	sql::ResultSet *resultSet = mysql_db_conn_->ExecuteQuery("select * from role");
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
	char str_sql[256] = {0};
	sprintf(str_sql, "select * from role where role_name=\"%s\" and account=\"%s\"", player_info.role_name.c_str(), player_info.account.c_str());
	LOG_DEBUG("%s", str_sql);
	sql::ResultSet *resultSet = mysql_db_conn_->ExecuteQuery(str_sql);
	if (!resultSet) {
		LOG_ERROR("create_player role_name = %s existed", player_info.role_name.c_str());
		return -1;
	}

	//从global表查询当前role_id最大值
	resultSet = mysql_db_conn_->ExecuteQuery("select * from global where type='role_id'");
	if (!resultSet) {
		LOG_ERROR("find from global type='role_id' not existed");
		return -1;
	}

	int order = resultSet->getInt64("role_id") + 1;
	int64_t agent = agent_num_ * 10000000000000L;
	int64_t server = server_num_ * 1000000000L;
	int64_t role_id = agent + server + order;
	sprintf(str_sql, "update global set value=%d where type='role_id'", order);
	LOG_DEBUG("%s", str_sql);
	mysql_db_conn_->ExecuteUpdate(str_sql);

	player_info.role_id = role_id;
	player_info.agent_num = agent_num_;
	player_info.server_num = server_num_;
	int now_sec = Time_Value::gettimeofday().sec();

	sprintf(str_sql, "insert into role (role_id, role_name, account, agent_num, server_num, level, gender, career, last_sign_in_time, last_sign_out_time) values "
			"(%ld, %s, %s, %d, %d, %d, %d, %d, %d, %d)", player_info.role_id, player_info.role_name.c_str(), player_info.account.c_str(), player_info.agent_num,
			player_info.server_num, 1, player_info.gender, player_info.career, now_sec, now_sec);
	LOG_DEBUG("%s", str_sql);
	mysql_db_conn_->Execute(str_sql);

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
	return 0;
}
