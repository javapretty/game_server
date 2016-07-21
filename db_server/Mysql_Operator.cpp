/*
 * Mysql_Operator.cpp
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#include "Mysql_Operator.h"
#include "Msg_Define.h"
#include "Log.h"

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

int Mysql_Operator::init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname) {
	mysql_poolname_ = dbpoolname;
	MYSQL_DB_MANAGER->Init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(mysql_poolname_);

	return 0;
}
