/*
 * DB_Record.cpp
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include "DB_Record.h"
#include "Server_Config.h"
#include "Block_Buffer.h"
#include "Log.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Msg_Define.h"

DB_Record::DB_Record(void)
: mysql_on_off_(true),
  mysql_port_(0),
  mysql_err_times_(0),
  driver_(0),
  conn_(0),
  mysql_db_conn_(NULL)
{ }


DB_Record::~DB_Record(void) {
	MYSQL_DB_MANAGER->RelDBConn(mysql_db_conn_);
}

#ifdef LOCAL_DEBUG
const int DB_Record::collector_max_num = 5;
const Time_Value DB_Record::collector_timeout = Time_Value(20, 0);
#else
const int DB_Record::collector_max_num = 1000;
const Time_Value DB_Record::collector_timeout = Time_Value(5, 0);
#endif
const int DB_Record::mysql_err_max_times = 20;

int DB_Record::set(std::string ip, int port, std::string &user, std::string &passwd) {
	mysql_ip_ = ip;
	mysql_port_ = port;
	mysql_user_ = user;
	mysql_pw_ = passwd;
	mysql_dbname_ = "log";
	mysql_poolname_ = "log_pool";
	return 0;
}

int DB_Record::init(void) {
	mysql_on_off_ = true;
	MYSQL_DB_MANAGER->Init(mysql_ip_, mysql_port_, mysql_user_, mysql_pw_, mysql_dbname_, mysql_poolname_, 16);
	mysql_db_conn_ = MYSQL_DB_MANAGER->GetDBConn(mysql_poolname_);
	init_collector();

	return 0;
}

int DB_Record::tick(Time_Value &now) {
	tick_collector(now);
	return 0;
}

void DB_Record::mysql_err_handler(void) {
	if (++mysql_err_times_ > mysql_err_max_times) {
		generate_mysql_err_file();
		mysql_err_times_ = 0;
	}
}

void DB_Record::generate_mysql_err_file(void) {
	FILE *fp = fopen("./mysql_err", "w");
	if (! fp) {
		LOG_ERROR("fopen mysql_err error");
		return;
	}
	fclose(fp);
}

int DB_Record::execute_collector(Data_Collector &collector) {
	if (! mysql_on_off_) /// MySQL off
		return -1;
	//add
	try {
		LOG_DEBUG("execute SQL=[%s]", collector.sql_str_.c_str());
		mysql_db_conn_->Execute(collector.sql_str_.c_str());
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		mysql_err_handler();
		process_mysql_errcode(err_code);
		return -1;
	}

	return 0;
}

int DB_Record::tick_collector(Time_Value &now) {
	if (test_collector_.is_timeout(now)) {
		execute_collector(test_collector_);
		test_collector_.reset_used();
	}
	if (loginout_collector_.is_timeout(now)) {
		execute_collector(loginout_collector_);
		loginout_collector_.reset_used();
	}

	return 0;
}

int DB_Record::cond_execute_collector(Data_Collector &collector) {
	if (collector.num_ > collector.max_num_) {
		execute_collector(collector);
		collector.reset_used();
	}
	return 0;
}

int DB_Record::process_mysql_errcode(int err_code) {
	switch (err_code) {
	default : {
		break;
	}
	}

	return 0;
}

void DB_Record::init_collector(void) {
	init_loginout_collector();
}

void DB_Record::init_loginout_collector(void) {
	std::string insert_head("INSERT INTO loginout "
			"(`role_id`,`role_name`,`account`,`level`,`client_ip`,`login_time`,`logout_time`) "
			"VALUES");
	loginout_collector_.set(insert_head, collector_max_num, collector_timeout);
}

int DB_Record::process_180001(int msg_id, int status, Block_Buffer &buf) {
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
