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
	static Mysql_Operator *instance(void);
	Mysql_DB_Conn *connection(void) { return mysql_db_conn_; }

	int init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname);

private:
	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);
	Mysql_Operator(const Mysql_Operator &);
	const Mysql_Operator &operator=(const Mysql_Operator &);

private:
	static Mysql_Operator *instance_;
	std::string mysql_poolname_;
	Mysql_DB_Conn* mysql_db_conn_;
};

#define MYSQL_INSTANCE			Mongo_Operator::instance()
#define MYSQL_CONNECTION		MYSQL_INSTANCE->connection()

#endif /* MYSQL_OPERATOR_H_ */
