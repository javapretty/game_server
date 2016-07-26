/*
 * Log_DB.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#ifndef LOG_DB_H_
#define LOG_DB_H_

#include "Mysql_Conn.h"
#include "Block_Buffer.h"

class Block_Buffer;

class Log_DB {
public:
	Log_DB(void);
	virtual ~Log_DB(void);

	int init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname);
	int process_180001(int msg_id, int status, Block_Buffer &buf);

private:
	std::string mysql_poolname_;
	Mysql_Conn* mysql_conn_;
};

#endif /* LOG_DB_H_ */
