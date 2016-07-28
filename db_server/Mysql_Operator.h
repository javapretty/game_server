/*
 * Mysql_Operator.h
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#ifndef MYSQL_OPERATOR_H_
#define MYSQL_OPERATOR_H_

#include "Mysql_Conn.h"
#include "Public_Struct.h"

class Mysql_Operator {
public:
	static Mysql_Operator *instance(void);
	Mysql_Conn *connection(void) { return mysql_conn_; }

	int init(void);
	int load_db_cache(void);
	int64_t create_player(Game_Player_Info &player_info);

private:
	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);
	Mysql_Operator(const Mysql_Operator &);
	const Mysql_Operator &operator=(const Mysql_Operator &);

private:
	static Mysql_Operator *instance_;
	Mysql_Conn* mysql_conn_;

	int agent_num_;				//代理编号
	int server_num_;			//服务器编号
};

#define MYSQL_INSTANCE		Mysql_Operator::instance()
#define MYSQL_CONNECTION	MYSQL_INSTANCE->connection()

#endif /* MYSQL_OPERATOR_H_ */
