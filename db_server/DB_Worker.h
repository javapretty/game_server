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
#include "Msg_Define.h"

class DB_Worker: public Thread {
public:
	typedef Block_List<Thread_Mutex> Data_List;

	DB_Worker(void);
	virtual ~DB_Worker(void);

	enum {
		PLAYER_CHANGE = 1,
		HERO_CHANGE,
		BAG_CHANGE,
		MAIL_CHANGE,
		SHOP_CHANGE,
		CHANGE_END
	};

	//玩家数据处理
	int load_player_data(int64_t role_id, Player_Data &player_data);
	int save_player_data(Player_Data &player_data);
	int set_player_data_change(bool change, Player_Data &player_data);

	/// 通用消息
	int push_data_block(Block_Buffer *buf);
	/// 按帐号加载玩家数据
	int push_load_player(Block_Buffer *buf);
	/// 创建角色
	int push_create_player(Block_Buffer *buf);
	/// 保存玩家全部数据
	int push_save_player(Block_Buffer *buf);

	void run_handler(void);
	void process_list(void);
	int process_data_block(Block_Buffer *buf);
	/// 加载db_cache
	int process_load_db_cache(int cid);
	/// 加载玩家数据
	int process_load_player(int cid, Account_Info &account_info);
	/// 创建新角色
	int process_create_player(int cid, Game_Player_Info &player_info);
	/// 保存玩家数据
	int process_save_player(int cid, Player_Data &player_data);
	/// 保存邮件信息
	int process_save_mail(MSG_150004 &msg);
	/// 获取公会信息
	int process_load_public_info(int cid, MSG_150101 &msg);
	/// 保存公会信息
	int process_save_guild(MSG_150102 &msg);
	/// 删除公会数据
	int process_drop_guild(MSG_150103 &msg);
private:
	Data_List data_list_;						///通用数据保存列表
	Data_List load_player_list_; 		///等待加载的玩家列表
	Data_List create_player_list_;		///等待创建的玩家列表
	Data_List save_player_list_; 		///等待保存的玩家数据列表
};

////////////////////////////////////////////////////////////////////////////////

inline int DB_Worker::push_data_block(Block_Buffer *buf) {
	data_list_.push_back(buf);
	return 0;
}

inline int DB_Worker::push_load_player(Block_Buffer *buf) {
	load_player_list_.push_back(buf);
	return 0;
}

inline int DB_Worker::push_create_player(Block_Buffer *buf) {
	create_player_list_.push_back(buf);
	return 0;
}

inline int DB_Worker::push_save_player(Block_Buffer *buf) {
	save_player_list_.push_back(buf);
	return 0;
}

#endif /* DB_WORKER_H_ */
