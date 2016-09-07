/*
 * Mysql_Operator.h
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#ifndef MYSQL_OPERATOR_H_
#define MYSQL_OPERATOR_H_

#include "Mysql_Conn.h"
#include "Public_Struct.h"

class Mysql_Operator {
public:
	typedef boost::unordered_map<int64_t, Mysql_Conn *> Connection_Map;

	static Mysql_Operator *instance(void);
	Mysql_Conn *connection(void);

	int init(void);
	int load_db_cache(void);

	int64_t generate_id(std::string type);
	int64_t create_player(Create_Role_Info &role_info);
	int64_t create_guild(Create_Guild_Info &guild_info);

private:
	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);
	Mysql_Operator(const Mysql_Operator &);
	const Mysql_Operator &operator=(const Mysql_Operator &);

private:
	static Mysql_Operator *instance_;
	Connection_Map connection_map_;
	MUTEX connection_map_lock_;

	int agent_num_;				//代理编号
	int server_num_;			//服务器编号
};

#define MYSQL_INSTANCE		Mysql_Operator::instance()
#define MYSQL_CONNECTION	MYSQL_INSTANCE->connection()

#endif /* MYSQL_OPERATOR_H_ */
