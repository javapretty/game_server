/*
 * Log_DB.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Log.h"
#include "Server_Config.h"
#include "Log_DB.h"

Log_DB::Log_DB(void) :
	mysql_conn_(nullptr),
	db_struct_id_map_(get_hash_table_size(512)),
	db_struct_name_map_(get_hash_table_size(512)) { }

Log_DB::~Log_DB(void) {
	MYSQL_MANAGER->rel_mysql_conn(mysql_conn_);
}

Log_DB *Log_DB::instance_;

Log_DB *Log_DB::instance(void) {
	if (instance_ == 0)
		instance_ = new Log_DB;
	return instance_;
}

int Log_DB::init(void) {
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_misc == Json::Value::null) {
		LOG_FATAL("db init, server_misc null");
		return -1;
	}

	//连接mysql
	std::string ip(server_misc["mysql_log"]["ip"].asString());
	int port = server_misc["mysql_log"]["port"].asInt();
	std::string user(server_misc["mysql_log"]["user"].asString());
	std::string password(server_misc["mysql_log"]["password"].asString());
	std::string dbname(server_misc["mysql_log"]["dbname"].asString());
	std::string dbpoolname(server_misc["mysql_log"]["dbpoolname"].asString());
	MYSQL_MANAGER->init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_conn_ = MYSQL_MANAGER->get_mysql_conn(dbpoolname);

	load_struct(server_misc["log_db_struct_path"].asString().c_str(), LOG_STRUCT, db_struct_id_map_, db_struct_name_map_);
	return 0;
}
