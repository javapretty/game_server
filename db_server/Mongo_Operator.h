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
	typedef std::set<int> Int_Set;
	typedef boost::unordered_map<int, Int_Set> Int_IntSet_Map;

	static Mongo_Operator *instance(void);
	mongo::DBClientConnection &connection(void);

	int init(void);
	int create_index(void);
	int load_db_cache(int cid);

	int64_t create_init_player(Game_Player_Info &player_info);
	bool role_exist(std::string &account, const int agent_num, const int server_num);
	int64_t get_role_id(const std::string &account, const int agent_num, const int server_num);

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

	int agent_num_; 									//代理编号
	int server_num_; 									//服编号
	Int_IntSet_Map server_map_;				//代理-服编号
};

#define MONGO_INSTANCE			Mongo_Operator::instance()
#define MONGO_CONNECTION		MONGO_INSTANCE->connection()

#endif /* DB_OPERATOR_H_ */
