/*
 * DB_Worker.h
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#ifndef DB_WORKER_H_
#define DB_WORKER_H_

#include "Public_Struct.h"
#include "Thread.h"
#include "Thread_Mutex.h"
#include "Block_List.h"

class DB_Worker: public Thread {
public:
	typedef Block_List<Thread_Mutex> Data_List;

	DB_Worker(void);
	virtual ~DB_Worker(void);

	void run_handler(void);
	void process_list(void);

	//其他数据库消息
	int process_data_block(Block_Buffer *buffer);
	//创建角色
	int process_create_player(int cid, Create_Role_Info &role_info);
	//加载玩家数据
	int process_load_player(int cid, std::string &account);
	//保存玩家数据
	int process_save_player(int cid, int status, Block_Buffer &buffer);
	// 创建公会
	int process_create_guild(int cid, Create_Guild_Info &guild_info);
	// 加载master数据
	int process_load_master(int cid);

	//玩家数据处理
	int create_player_data(int64_t role_id, Block_Buffer &buffer);
	int load_player_data(int64_t role_id, Block_Buffer &buffer);
	int save_player_data(Block_Buffer &buffer);

	// 创建角色
	int push_create_player(Block_Buffer *buffer);
	//加载玩家数据
	int push_load_player(Block_Buffer *buffer);
	//保存玩家数据
	int push_save_player(Block_Buffer *buffer);
	//其他数据库消息
	int push_data_block(Block_Buffer *buffer);

private:
	Data_List create_player_list_;	//创建的玩家数据列表
	Data_List load_player_list_; 		//加载的玩家数据列表
	Data_List save_player_list_; 		//保存的玩家数据列表
	Data_List data_list_;						//其他数据库消息消息列表
};

////////////////////////////////////////////////////////////////////////////////
inline int DB_Worker::push_create_player(Block_Buffer *buffer) {
	create_player_list_.push_back(buffer);
	return 0;
}

inline int DB_Worker::push_load_player(Block_Buffer *buffer) {
	load_player_list_.push_back(buffer);
	return 0;
}

inline int DB_Worker::push_save_player(Block_Buffer *buffer) {
	save_player_list_.push_back(buffer);
	return 0;
}

inline int DB_Worker::push_data_block(Block_Buffer *buffer) {
	data_list_.push_back(buffer);
	return 0;
}

#endif /* DB_WORKER_H_ */
