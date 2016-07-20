/*
 * Mysql_Operator.cpp
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#include "Mysql_Operator.h"
#include "Msg_Define.h"
#include "Log.h"

Mysql_Operator::Mysql_Operator(void)
: mysql_db_conn_(NULL)
{ }


Mysql_Operator::~Mysql_Operator(void) {
	MYSQL_DB_MANAGER->RelDBConn(mysql_db_conn_);
}

int Mysql_Operator::init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname) {
	mysql_poolname_ = dbpoolname;
	MYSQL_DB_MANAGER->Init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(mysql_poolname_);

	return 0;
}

int Mysql_Operator::execute_sql(std::string &sql_str) {
	try {
		LOG_DEBUG("execute SQL=[%s]", sql_str.c_str());
		mysql_db_conn_->Execute(sql_str.c_str());
	} catch (sql::SQLException &e) {
		LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", e.getErrorCode(), e.getSQLState().c_str(), e.what());
		return -1;
	}

	return 0;
}
