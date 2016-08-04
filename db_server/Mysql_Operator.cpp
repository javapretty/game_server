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

Mysql_Operator::Mysql_Operator(void) : mysql_conn_(nullptr), agent_num_(0), server_num_(0) { }

Mysql_Operator::~Mysql_Operator(void) {
	MYSQL_MANAGER->rel_mysql_conn(mysql_conn_);
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
	MYSQL_MANAGER->init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_conn_ = MYSQL_MANAGER->get_mysql_conn(dbpoolname);

	sql::ResultSet *result = mysql_conn_->execute_query("select * from global where type='role_id'");
	if (result && result->rowsCount() <= 0) {
		mysql_conn_->execute("insert into global(type, value) values ('role_id', 0)");
	}
	result = mysql_conn_->execute_query("select * from global where type='guild_id'");
	if (result && result->rowsCount() <= 0) {
		mysql_conn_->execute("insert into global(type, value) values ('guild_id', 0)");
	}

	load_db_cache();
	return 0;
}

int Mysql_Operator::load_db_cache(void) {
	sql::ResultSet *result = mysql_conn_->execute_query("select * from role");
	if (result) {
		while(result->next()) {
			Player_DB_Cache db_cache;
			db_cache.role_id = result->getInt64("role_id");
			db_cache.role_name = result->getString("role_name");
			db_cache.account = result->getString("account");
			db_cache.agent_num = result->getInt("agent_num");
			db_cache.server_num = result->getInt("server_num");
			db_cache.level = result->getInt("level");
			DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
			DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
		}
	}
	LOG_INFO("***********load db_cache player count:%d*************", DB_MANAGER->db_cache_account_map().size());
	return 0;
}

int64_t Mysql_Operator::create_player(Create_Role_Info &role_info) {
	char str_sql[256] = {0};
	sprintf(str_sql, "select * from role where account='%s' and role_name='%s'", role_info.account.c_str(), role_info.role_name.c_str());
	sql::ResultSet *result = mysql_conn_->execute_query(str_sql);
	if (result && result->rowsCount() > 0) {
		LOG_ERROR("create_player account = %s role_name = %s existed", role_info.account.c_str(), role_info.role_name.c_str());
		return -1;
	}

	//从global表查询当前role_id最大值
	result = mysql_conn_->execute_query("select * from global where type='role_id'");
	if (result && result->rowsCount() <= 0) {
		LOG_ERROR("find from global type='role_id' not existed");
		return -1;
	}

	int order = 1;
	while(result->next()) {
		order = result->getInt64("value") + 1;
		break;
	}
	int64_t agent = agent_num_ * 10000000000000L;
	int64_t server = server_num_ * 1000000000L;
	int64_t role_id = agent + server + order;
	sprintf(str_sql, "update global set value=%d where type='role_id'", order);
	mysql_conn_->execute_update(str_sql);

	int now_sec = Time_Value::gettimeofday().sec();
	sprintf(str_sql, "insert into role (role_id, role_name, account, agent_num, server_num, level, gender, career, create_time, login_time) values (%ld, '%s', '%s', %d, %d, %d, %d, %d, %d, %d)",
			role_id, role_info.role_name.c_str(), role_info.account.c_str(), agent_num_,
			server_num_, 1, role_info.gender, role_info.career, now_sec, now_sec);
	mysql_conn_->execute(str_sql);

	Player_DB_Cache db_cache;
	db_cache.role_id = role_id;
	db_cache.role_name = role_info.role_name;
	db_cache.account = role_info.account;
	db_cache.agent_num = agent_num_;
	db_cache.server_num = server_num_;
	db_cache.level = 1;
	DB_MANAGER->db_cache_id_map().insert(std::make_pair(db_cache.role_id, db_cache));
	DB_MANAGER->db_cache_account_map().insert(std::make_pair(db_cache.account, db_cache));
	LOG_INFO("***************create role, role_id:%ld, db_cache count:%d***************", role_id, DB_MANAGER->db_cache_account_map().size());
	return role_id;
}

int64_t Mysql_Operator::create_guild(Create_Guild_Info &guild_info) {
	char str_sql[256] = {0};
	sprintf(str_sql, "select * from guild where guild_name='%s'", guild_info.guild_name.c_str());
	sql::ResultSet *result = mysql_conn_->execute_query(str_sql);
	if (result && result->rowsCount() > 0) {
		LOG_ERROR("create_guild guild_name = %s existed", guild_info.guild_name.c_str());
		return -1;
	}

	//从global表查询当前role_id最大值
	result = mysql_conn_->execute_query("select * from global where type='guild_id'");
	if (result && result->rowsCount() <= 0) {
		LOG_ERROR("find from global type='guild_id' not existed");
		return -1;
	}

	int order = 1;
	while(result->next()) {
		order = result->getInt64("value") + 1;
		break;
	}
	int64_t agent = agent_num_ * 10000000000000L;
	int64_t server = server_num_ * 1000000000L;
	int64_t guild_id = agent + server + order;
	sprintf(str_sql, "update global set value=%d where type='guild_id'", order);
	mysql_conn_->execute_update(str_sql);

	int now_sec = Time_Value::gettimeofday().sec();
	sprintf(str_sql, "insert into guild (guild_id, guild_name, chief_id, create_time) values (%ld, '%s', %ld, %d)",
			guild_id, guild_info.guild_name.c_str(), guild_info.chief_id, now_sec);
	mysql_conn_->execute(str_sql);

	LOG_INFO("***************create guild,guild_id:%ld***************", guild_id);
	return guild_id;
}
