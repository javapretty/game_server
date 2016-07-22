/*
 * Mysql_Operator.cpp
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Mysql_Operator.h"
#include "Server_Config.h"

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

int Mysql_Operator::init() {
	const Json::Value &mysql_game = SERVER_CONFIG->server_misc()["mysql_game"];
	if (mysql_game == Json::Value::null) {
		LOG_FATAL("server_misc config error");
	}

	std::string ip(mysql_game["ip"].asString());
	int port = mysql_game["port"].asInt();
	std::string user(mysql_game["user"].asString());
	std::string password(mysql_game["password"].asString());
	std::string dbname(mysql_game["dbname"].asString());
	std::string dbpoolname(mysql_game["dbpoolname"].asString());
	MYSQL_DB_MANAGER->Init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(dbpoolname);

	return 0;
}
