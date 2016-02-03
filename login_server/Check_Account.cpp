/*
 * check_accout.cpp
 *
 *  Created on: 2016年1月4日
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "Check_Account.h"
#include "Configurator.h"
#include "Log.h"

Check_Account::Check_Account():
mysql_db_conn_(NULL)
{}

Check_Account::~Check_Account() { }

int Check_Account::connect_mysql_db() {
	const Json::Value &server_maintainer = CONFIG_INSTANCE->server_maintainer();
	if (server_maintainer == Json::Value::null) {
		MSG_ABORT("server_maintainer == Json::Value::null");
	}
	std::string mysql_ip(server_maintainer["mysql_server"]["ip"].asString());
	int mysql_port = server_maintainer["mysql_server"]["port"].asInt();
	std::string mysql_user(server_maintainer["mysql_server"]["user"].asString());
	std::string mysql_pw(server_maintainer["mysql_server"]["password"].asString());
	std::string db_name("account");
	std::string pool_name("account_pool");

	MYSQL_DB_MANAGER->Init(mysql_ip, mysql_port, mysql_user, mysql_pw, db_name, pool_name, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(pool_name);
	return 0;
}

void Check_Account::release_mysql_conn() {
	MYSQL_DB_MANAGER->RelDBConn(mysql_db_conn_);
}

//注册
int Check_Account::client_register(std::string& account, std::string& password){
	int ret = 0;
	try{
		std::string sql_query = "insert into account_info values (\'" + account + "\', \'" +  password +"\')";
		ret = mysql_db_conn_->Execute(sql_query.c_str());
	}catch(sql::SQLException &e){
		int err_code = e.getErrorCode();
		MSG_DEBUG("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		ret = -1;
	}

	return ret;
}

//登录
int Check_Account::client_login(std::string& account, std::string& password){
	int ret = 0;
	try{
		std::string sql_query = "select count(1) from account_info where account = \'" + account + "\' and password = \'" + password + "\'";
		sql::ResultSet* res = mysql_db_conn_->ExecuteQuery(sql_query.c_str());
		int cnt = 0;
		while (res->next()) {
			cnt = res->getInt(1);
		}
		delete res;

		if (cnt <= 0){
			ret = -1;
		}
	}catch(sql::SQLException &e){
		int err_code = e.getErrorCode();
		LOG_DEBUG("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		ret = -1;
	}

	return ret;
}
