/*
 * check_accout.h
 *
 *  Created on: 2016年1月4日
 *      Author: zhangyalei
 */

#ifndef CHECK_ACCOUNT_H_
#define CHECK_ACCOUNT_H_

#include <string>
#include "Mysql_Conn.h"

class Block_Buffer;

class Check_Account{
public:
	Check_Account();
	~Check_Account();

	int connect_mysql_db();
	void release_mysql_conn();

	int client_register(std::string& account, std::string& password);
	int client_login(std::string& account, std::string& password);

private:
	Mysql_DB_Conn* mysql_db_conn_;
};

#endif /* CHECK_ACCOUNT_H_ */
