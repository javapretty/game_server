/*
 * Log_DB.cpp
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Log_DB.h"
#include "Msg_Define.h"
#include "Server_Config.h"
#include "Block_Buffer.h"
#include "Log.h"

Log_DB::Log_DB(void)
: mysql_on_off_(true),
  mysql_port_(0),
  driver_(0),
  conn_(0),
  mysql_db_conn_(NULL)
{ }


Log_DB::~Log_DB(void) {
	MYSQL_DB_MANAGER->RelDBConn(mysql_db_conn_);
}

const int Log_DB::collector_max_num = 5;
const Time_Value Log_DB::collector_timeout = Time_Value(5, 0);

int Log_DB::set(std::string ip, int port, std::string &user, std::string &passwd) {
	mysql_ip_ = ip;
	mysql_port_ = port;
	mysql_user_ = user;
	mysql_pw_ = passwd;
	mysql_dbname_ = "log";
	mysql_poolname_ = "log_pool";
	return 0;
}

int Log_DB::init(void) {
	mysql_on_off_ = true;
	MYSQL_DB_MANAGER->Init(mysql_ip_, mysql_port_, mysql_user_, mysql_pw_, mysql_dbname_, mysql_poolname_, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(mysql_poolname_);
	init_collector();

	return 0;
}

int Log_DB::tick(Time_Value &now) {
	tick_collector(now);
	return 0;
}

int Log_DB::execute_collector(Data_Collector &collector) {
	if (! mysql_on_off_)
		return -1;

	try {
		LOG_DEBUG("execute SQL=[%s]", collector.sql_str_.c_str());
		mysql_db_conn_->Execute(collector.sql_str_.c_str());
	} catch (sql::SQLException &e) {
		LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", e.getErrorCode(), e.getSQLState().c_str(), e.what());
		return -1;
	}

	return 0;
}

int Log_DB::tick_collector(Time_Value &now) {
	if (loginout_collector_.is_timeout(now)) {
		execute_collector(loginout_collector_);
		loginout_collector_.reset_used();
	}

	return 0;
}

int Log_DB::cond_execute_collector(Data_Collector &collector) {
	if (collector.num_ > collector.max_num_) {
		execute_collector(collector);
		collector.reset_used();
	}
	return 0;
}

void Log_DB::init_collector(void) {
	init_loginout_collector();
}

void Log_DB::init_loginout_collector(void) {
	std::string insert_head("INSERT INTO loginout "
			"(`role_id`,`role_name`,`account`,`level`,`client_ip`,`login_time`,`logout_time`) "
			"VALUES");
	loginout_collector_.set(insert_head, collector_max_num, collector_timeout);
}

int Log_DB::process_180001(int msg_id, int status, Block_Buffer &buf) {
	MSG_180001 msg;
	msg.deserialize(buf);

	Loginout_Table table;
	table.role_id_.data_ = msg.role_id;
	table.role_name_.data_ = msg.role_name;
	table.account_.data_ = msg.account;
	table.level_.data_ = msg.level;
	table.client_ip_.data_ = msg.client_ip;
	table.login_time_.data_ = msg.login_time;
	table.logout_time_.data_ = msg.logout_time;

	table.append_insert_content(loginout_collector_);
	cond_execute_collector(loginout_collector_);
	return 0;
}
