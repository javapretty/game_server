/*
 * DB_Manager.h
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include "DB_Worker.h"
#include "Log.h"
#include "Object_Pool.h"
#include "Block_Buffer.h"
#include "Thread_Mutex.h"
#include <vector>

class DB_Manager {
public:
	typedef Object_Pool<DB_Worker, Thread_Mutex> DB_Worker_Pool;
	typedef std::vector<DB_Worker *> DB_Worker_Vector;

	static DB_Manager *instance(void);
	static void destroy(void);

	int set(int num);
	int init(void);
	int start(void);

	int push_data_block(Block_Buffer *buf);
	int send_data_block(int cid, Block_Buffer &buf);

	void object_pool_size(void);
	void free_pool_cache(void);

private:
	DB_Manager(void);
	virtual ~DB_Manager(void);
	DB_Manager(const DB_Manager &);
	const DB_Manager &operator=(const DB_Manager &);

private:
	static DB_Manager *instance_;
	int work_num_;							//db工作线程数量
	int load_player_num_;				//加载数据的玩家数量
	int create_player_num_;			//创建的玩家数量
	int save_player_num_;				//保存数据的玩家数量
	int db_data_num_;						//保存数据库的消息数量

	DB_Worker_Pool db_worker_pool_;
	DB_Worker_Vector db_worker_vec_;
};

#define DB_MANAGER DB_Manager::instance()

////////////////////////////////////////////////////////////////////////////////

inline int DB_Manager::set(int num) {
	work_num_ = num;
	return 0;
}

inline void DB_Manager::object_pool_size(void) {
	MSG_DEBUG("db_worker_pool_ free = %d, used = %d", db_worker_pool_.free_obj_list_size(), db_worker_pool_.used_obj_list_size());
}

inline void DB_Manager::free_pool_cache(void) {
	db_worker_pool_.shrink_all();
}

#endif /* DB_MANAGER_H_ */
