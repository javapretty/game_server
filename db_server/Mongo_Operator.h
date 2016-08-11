/*
 * Mongo_Operator.h
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#ifndef DB_OPERATOR_H_
#define DB_OPERATOR_H_

#include "Public_Struct.h"
#include "Object_Pool.h"
#include "mongo/client/dbclient.h"

using namespace mongo;

class Mongo_Operator {
public:
	typedef boost::unordered_map<int64_t, DBClientConnection *> Connection_Map;
	typedef Object_Pool<DBClientConnection, Thread_Mutex> Connection_Pool;

	static Mongo_Operator *instance(void);
	mongo::DBClientConnection &connection(void);

	int init(void);
	int load_db_cache(void);

	int64_t generate_id(std::string type);
	int64_t create_player(Create_Role_Info &role_info);
	int64_t create_guild(Create_Guild_Info &guild_info);

private:
	Mongo_Operator(void);
	virtual ~Mongo_Operator(void);
	Mongo_Operator(const Mongo_Operator &);
	const Mongo_Operator &operator=(const Mongo_Operator &);

private:
	static Mongo_Operator *instance_;
	Connection_Pool connection_pool_;
	Connection_Map connection_map_;
	MUTEX connection_map_lock_;

	int agent_num_;					//代理编号
	int server_num_;				//服务器编号
};

#define MONGO_INSTANCE		Mongo_Operator::instance()
#define MONGO_CONNECTION	MONGO_INSTANCE->connection()

#endif /* DB_OPERATOR_H_ */
