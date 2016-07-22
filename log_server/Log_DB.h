/*
 * Log_DB.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#ifndef LOG_DB_H_
#define LOG_DB_H_

#include "Log_Fields.h"
#include "Mysql_Conn.h"
#include "Block_Buffer.h"
#include <string>

class Block_Buffer;

class Log_DB {
public:
	const static int collector_max_num;
	const static Time_Value collector_timeout;

	Log_DB(void);
	virtual ~Log_DB(void);

	int init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname);
	int tick(Time_Value &now);

	int process_180001(int msg_id, int status, Block_Buffer &buf);

private:
	/// 直接执行collector中的SQL语句
	int execute_collector(Data_Collector &collector);
	int tick_collector(Time_Value &now);
	/// collector中的数据条数num超过最大条数时执行该collector中的SQL语句
	int cond_execute_collector(Data_Collector &collector);
	void init_collector(void);

	void init_logout_collector(void);

private:
	bool mysql_on_off_;
	std::string mysql_poolname_;

	Data_Collector logout_collector_;
	Mysql_DB_Conn* mysql_db_conn_;
};

#endif /* LOG_DB_H_ */
