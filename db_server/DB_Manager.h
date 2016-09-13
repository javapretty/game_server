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
#include "Thread_Mutex.h"

class DB_Manager {
public:
	typedef Object_Pool<DB_Worker, Thread_Mutex> DB_Worker_Pool;
	typedef std::vector<DB_Worker *> DB_Worker_Vector;
	typedef boost::unordered_map<int64_t,Player_DB_Cache> DB_Cache_Id_Map;
	typedef boost::unordered_map<std::string,Player_DB_Cache> DB_Cache_Account_Map;

	static DB_Manager *instance(void);
	static void destroy(void);

	int init(int server_id);
	int start(void);

	int push_data_block(Block_Buffer *buf);
	int send_data_block(int cid, Block_Buffer &buf);

	inline Struct_Id_Map& db_struct_id_map() { return db_struct_id_map_; }
	inline Struct_Name_Map& db_struct_name_map() { return db_struct_name_map_; }
	inline DB_Cache_Id_Map& db_cache_id_map() { return db_cache_id_map_; }
	inline DB_Cache_Account_Map& db_cache_account_map() { return db_cache_account_map_; }

	inline Base_Struct *get_player_data_struct() {
		return db_struct_name_map_.find("Player_Data")->second;
	}
	inline Base_Struct *get_master_data_struct() {
		return db_struct_name_map_.find("Master_Data")->second;
	}

	int64_t create_player(Create_Role_Info &role_info);
	int64_t create_guild(Create_Guild_Info &role_info);

private:
	DB_Manager(void);
	virtual ~DB_Manager(void);
	DB_Manager(const DB_Manager &);
	const DB_Manager &operator=(const DB_Manager &);

private:
	static DB_Manager *instance_;

	DB_Worker_Pool db_worker_pool_;
	DB_Worker_Vector db_worker_vec_;

	Struct_Id_Map db_struct_id_map_;
	Struct_Name_Map db_struct_name_map_;
	DB_Cache_Id_Map db_cache_id_map_;
	DB_Cache_Account_Map db_cache_account_map_;

	int load_player_num_;				//加载数据的玩家数量
	int create_player_num_;			//创建的玩家数量
	int save_player_num_;				//保存数据的玩家数量
	int db_data_num_;						//保存数据库的消息数量
	int db_type_;								//数据库类型
};

#define DB_MANAGER DB_Manager::instance()

#endif /* DB_MANAGER_H_ */
