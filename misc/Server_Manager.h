/*
 * Server_Manager.h
 *
 *  Created on: Sep 7, 2016
 *      Author: zhangyalei
 */

#ifndef SERVER_MANAGER_H_
#define SERVER_MANAGER_H_

/*
 * 服务器状态
1、启动时间
2、发送总字节，接收总字节
3、进程玩家数量 登录用户数量
4、进程占用资源  cpu 内存
5、进程消息统计，未消息统计
6、单位时间消息统计

服务器操作
1、关闭重启服务器
 */

#include "Common_Func.h"
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

	int init_data(int server_id, std::string server_name);
	int server_id(void) { return server_id_; }

	virtual void run_handler(void);
	virtual int process_list(void);

	int bind_cid_player(int cid, Player *player);
	Player* find_cid_player(int cid);

	int bind_role_id_player(int64_t role_id, Player *player);
	Player* find_role_id_player(int64_t role_id);

	int bind_account_player(std::string &account, Player *player);
	Player* find_account_player(std::string &account);

	//主动关闭处理
	int self_close_process(void);

	//关闭客户端连接
	virtual int close_client(int gate_cid, int player_cid, int error_code);
	virtual int recycle_player(int gate_cid, int player_cid);
	virtual int free_cache(void);

	//定时器处理
	int tick(void);
	inline const Time_Value &tick_time(void) { return tick_time_; }
	int close_list_tick(Time_Value &now);			//关闭连接定时器
  int player_tick(Time_Value &now);						//玩家定时器
	int server_info_tick(Time_Value &now);			//收集服务器信息定时器

	//服务器信息收集
	virtual void get_server_info(void);
	virtual void print_server_info(void);

	//消息统计
	void print_msg_info(void);
	inline void add_msg_count(int msg_id) {
		if (msg_count_) {
			++(msg_count_map_[msg_id]);
		}
	}
	inline void add_recv_bytes(int bytes) {
		if (msg_count_) {
			total_recv_bytes += bytes;
		}
	}
	inline void add_send_bytes(int bytes) {
		if (msg_count_) {
			total_send_bytes += bytes;
		}
	}

protected:
	Player_Cid_Map player_cid_map_;
	Player_RoleId_Map player_role_id_map_;
	Player_Account_Map player_account_map_;

private:
	Block_Pool block_pool_;
	Close_List close_list_; 				//连接cid在2秒后投递到通信层关闭

	int server_id_;									//服务器id
	int server_status_;							//服务器状态
	std::string server_name_;				//服务器名字
	Time_Value server_start_time_;	//服务器开始时间
	Time_Value tick_time_;						//服务器tick时间
	Time_Value player_tick_;
	Time_Value server_info_tick_;

	bool msg_count_;
	Msg_Count_Map msg_count_map_;
	int total_recv_bytes;						//总共接收的字节
	int total_send_bytes;						//总共发送的字节
};

#endif /* SERVER_MANAGER_H_ */
