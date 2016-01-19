// -*- C++ -*-
/*
 * DB_Record.h
 *
 *  Created on: Aug 29, 2012
 *      Author: zhangyalei
 */

#ifndef DB_RECORD_H_
#define DB_RECORD_H_

#include "My_DB_Fields.h"
#include "Time_Value.h"
#include "Mysql_Conn.h"
#include <string>

class Block_Buffer;

namespace sql {
class Driver;
class Connection;
}

class DB_Record {
public:
	const static int collector_max_num;
	const static Time_Value collector_timeout;
	const static int mysql_err_max_times;

	DB_Record(void);
	virtual ~DB_Record(void);
	int set(std::string ip, int port, std::string &user, std::string &passwd);
	int init(void);

	int tick(Time_Value &now);
	void mysql_err_handler(void);
	void generate_mysql_err_file(void);

	int process_185000(int msg_id, int status, Block_Buffer &buf);
	int process_185001(int msg_id, int status, Block_Buffer &buf);

private:
	int process_mysql_errcode(int err_code);

	/// 直接执行collector中的SQL语句
	int execute_collector(Data_Collector &collector);
	int tick_collector(Time_Value &now);
	/// collector中的数据条数num超过最大条数时执行该collector中的SQL语句
	int cond_execute_collector(Data_Collector &collector);
	void init_collector(void);

	void init_test_collector(void);
	void init_loginout_stream_collector(void);

private:
	bool mysql_on_off_;
	std::string mysql_ip_;
	int mysql_port_;
	std::string mysql_user_;
	std::string mysql_pw_;
	std::string mysql_dbname_;
	std::string mysql_poolname_;

	int mysql_err_times_; /// MySQL操作失败次数(包括connect, exec SQL操作等)

	sql::Driver *driver_;
	sql::Connection *conn_;

	Data_Collector test_collector_;
	Data_Collector loginout_stream_collector_;
	Mysql_DB_Conn* mysql_db_conn_;
};

#endif /* DB_RECORD_H_ */
