/*
 * Mysql_Operator.h
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#ifndef MYSQL_OPERATOR_H_
#define MYSQL_OPERATOR_H_

#include "Mysql_Conn.h"
#include <string>

class Mysql_Operator {
public:
	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);

	int init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname);

	int execute_sql(std::string &sql_str);

private:
	std::string mysql_poolname_;

	Mysql_DB_Conn* mysql_db_conn_;
};

#endif /* MYSQL_OPERATOR_H_ */
