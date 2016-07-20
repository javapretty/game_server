/*
 * Log_DB.cpp
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include "Log_DB.h"
#include "Msg_Define.h"
#include "Log.h"

Log_DB::Log_DB(void)
: mysql_on_off_(true),
  mysql_db_conn_(NULL)
{ }


Log_DB::~Log_DB(void) {
	MYSQL_DB_MANAGER->RelDBConn(mysql_db_conn_);
}

const int Log_DB::collector_max_num = 5;
const Time_Value Log_DB::collector_timeout = Time_Value(5, 0);

int Log_DB::init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname) {
	mysql_on_off_ = true;
	mysql_poolname_ = dbpoolname;
	MYSQL_DB_MANAGER->Init(ip, port, user, password, dbname, dbpoolname, 16);
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
