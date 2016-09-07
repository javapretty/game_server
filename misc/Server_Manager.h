/*
 * Server_Manager.h
 *
 *  Created on: Sep 7, 2016
 *      Author: zhangyalei
 */

#ifndef SERVER_MANAGER_H_
#define SERVER_MANAGER_H_

#include "Log.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Player.h"

class Server_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;
	typedef std::list<Close_Info> Close_List;

	typedef boost::unordered_map<int, Player* > Player_Cid_Map;
	typedef boost::unordered_map<int64_t, Player* > Player_RoleId_Map;
	typedef boost::unordered_map<std::string, Player *> Player_Account_Map;
	typedef boost::unordered_map<int, int> Msg_Count_Map;

public:
	Server_Manager(void);
	virtual ~Server_Manager(void);

	void run_handler(void);
	virtual int process_list(void);

	//主动关闭处理
	int self_close_process(void);

	inline void set_server_name(std::string server_name) { server_name_ = server_name; }
	inline int server_status(void) { return server_status_; }

	int bind_cid_player(int cid, Player *player);
	Player* find_cid_player(int cid);

	int bind_role_id_player(int64_t role_id, Player *player);
	Player* find_role_id_player(int64_t role_id);

	int bind_account_player(std::string &account, Player *player);
	Player* find_account_player(std::string &account);

	virtual int unbind_player(Player &player);

	/// 定时器处理
	int tick(void);
	inline const Time_Value &tick_time(void) { return tick_time_; }
	virtual int close_list_tick(Time_Value &now);			//处理关闭连接定时器
	virtual int player_tick(Time_Value &now);					//玩家定时器
	virtual int server_info_tick(Time_Value &now);			//收集服务器信息定时器

	virtual void get_server_info(void);
	virtual void free_cache(void);
	virtual void print_object_pool(void);

	void print_msg_count(void);
	inline void add_msg_count(int msg_id) {
		if (msg_count_onoff_) {
			++(msg_count_map_[msg_id]);
		}
	}

protected:
	Player_Cid_Map player_cid_map_;

private:
	Block_Pool block_pool_;

	Player_RoleId_Map player_role_id_map_;
	Player_Account_Map player_account_map_;

	std::string server_name_;
	int server_status_;
	Time_Value last_player_tick_;
	Time_Value last_server_info_tick_;
	Time_Value tick_time_;

	bool msg_count_onoff_;
	Msg_Count_Map msg_count_map_;
};

#endif /* SERVER_MANAGER_H_ */
