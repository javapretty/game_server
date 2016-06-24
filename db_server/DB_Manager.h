/*
 * DB_Manager.h
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include "DB_Worker.h"
#include "DB_Definition.h"
#include "Log.h"
#include "Object_Pool.h"
#include "Block_Buffer.h"
#include "Thread_Mutex.h"
#include <vector>
#include <map>

class DB_Manager {
public:
	typedef Object_Pool<DB_Worker, Thread_Mutex> DB_Worker_Pool;
	typedef std::vector<DB_Worker *> DB_Worker_Vector;
	typedef std::map<int32_t, DB_Definition *> DB_Id_Definition_Map;
	typedef std::map<std::string, DB_Definition *> DB_Name_Definition_Map;

	static DB_Manager *instance(void);
	static void destroy(void);

	int init(void);
	int start(void);

	int push_data_block(Block_Buffer *buf);
	int send_data_block(int cid, Block_Buffer &buf);

	void object_pool_size(void);
	void free_pool_cache(void);

	DB_Name_Definition_Map& db_name_definition_map();
	DB_Id_Definition_Map& db_id_definition_map();

	DB_Definition *get_player_data_definition();
	DB_Definition *get_public_data_definition();

private:
	DB_Manager(void);
	virtual ~DB_Manager(void);
	DB_Manager(const DB_Manager &);
	const DB_Manager &operator=(const DB_Manager &);
	int load_struct(const char *path);

private:
	static DB_Manager *instance_;
	int load_player_num_;				//加载数据的玩家数量
	int create_player_num_;			//创建的玩家数量
	int save_player_num_;				//保存数据的玩家数量
	int db_data_num_;						//保存数据库的消息数量

	DB_Worker_Pool db_worker_pool_;
	DB_Worker_Vector db_worker_vec_;
	DB_Id_Definition_Map db_id_definition_map_;
	DB_Name_Definition_Map db_name_definition_map_;
};

#define DB_MANAGER DB_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline void DB_Manager::object_pool_size(void) {
	LOG_INFO("db_worker_pool_ free = %d, used = %d", db_worker_pool_.free_obj_list_size(), db_worker_pool_.used_obj_list_size());
}

inline void DB_Manager::free_pool_cache(void) {
	db_worker_pool_.shrink_all();
}

inline DB_Manager::DB_Name_Definition_Map& DB_Manager::db_name_definition_map(){
	return db_name_definition_map_;
}

inline DB_Manager::DB_Id_Definition_Map& DB_Manager::db_id_definition_map(){
	return db_id_definition_map_;
}

inline DB_Definition *DB_Manager::get_player_data_definition(){
	return db_name_definition_map_.find("Player_Data")->second;
}

inline DB_Definition *DB_Manager::get_public_data_definition(){
	return db_name_definition_map_.find("Public_Data")->second;
}

#endif /* DB_MANAGER_H_ */
