/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef LOGIN_MANAGER_H_
#define LOGIN_MANAGER_H_

#include "Log.h"
#include "Block_Buffer.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Check_Account.h"
#include "Login_Player.h"


class Login_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Block_List<Thread_Mutex> Data_List;
	typedef std::list<Close_Info> Close_List;
	typedef boost::unordered_map<int, int> Msg_Count_Map;
	typedef std::vector<Ip_Info> Ip_Vec;

	typedef Object_Pool<Login_Player, Spin_Lock> Login_Player_Pool;
	typedef List<int, Thread_Mutex> Int_List;
	typedef boost::unordered_map<int, Login_Player *> Login_Player_Map;
	typedef boost::unordered_map<std::string, Login_Player *> Login_Player_Account_Map;

public:
	enum {
		STATUS_NORMAL = 1,
		STATUS_CLOSING = 2,
	};

	static Login_Manager *instance(void);
	int init(void);
	void run_handler(void);

	/// 服务器状态
	inline int server_status(void) { return server_status_; };

	int init_gate_ip(void);
	void get_gate_ip(std::string &account, std::string &ip, int &port);

	Login_Player *pop_login_player(void);
	int push_login_player(Login_Player *player);

	int bind_account_login_player(std::string& account, Login_Player *player);
	int unbind_account_login_player(std::string& account);
	Login_Player *find_account_login_player(std::string& account) ;

	int bind_cid_login_player(int cid, Login_Player *player);
	int unbind_cid_login_player(int cid);
	Login_Player *find_cid_login_player(int cid);

	int unbind_login_player(Login_Player &player);

	/// 发送数据接口
	int send_to_client(int player_cid, Block_Buffer &buf);
	int send_to_gate(int gate_cid, Block_Buffer &buf);
	/// 关闭连接
	int close_client(int player_cid);
	/// 主动关闭处理
	int self_close_process(void);

	/// 通信层投递消息到Login_Manager
	void push_drop_cid(int cid);
	int push_login_client_data(Block_Buffer *buf);
	int push_login_gate_data(Block_Buffer *buf);
	int push_self_loop_message(Block_Buffer &msg_buf);

	/// 消息处理
	int process_list();
	void process_drop_cid(int cid);

	/// 定时器处理
	int tick(void);
	/// 返回上次tick的绝对时间, 最大误差有100毫秒,主要为减少系统调用gettimeofday()调用次数
	inline const Time_Value &tick_time(void) { return tick_time_; };
	int close_list_tick(Time_Value &now);
	int server_info_tick(Time_Value &now);
	int player_tick(Time_Value &now);
	void object_pool_tick(Time_Value &now);

	void get_server_info(Block_Buffer &buf);
	void object_pool_size(void);
	void free_cache(void);

	/// 统计内部消息量
	void set_msg_count_onoff(int v);
	void print_msg_count(void);
	void inner_msg_count(Block_Buffer &buf);
	void inner_msg_count(int msg_id);

	Check_Account &check_account(void);

private:
	Login_Manager(void);
	virtual ~Login_Manager(void);
	Login_Manager(const Login_Manager &);
	const Login_Manager &operator=(const Login_Manager &);

private:
	static Login_Manager *instance_;

	Block_Pool block_pool_;

	Data_List login_client_data_list_;				///client-->login
	Data_List login_gate_data_list_;		///login-->connector
	Data_List self_loop_block_list_; 				/// self_loop_block_list
	Close_List close_list_; /// 其中的连接cid在n秒后投递到通信层关闭

	Server_Info login_client_server_info_;
	Server_Info login_gate_server_info_;

	Login_Player_Pool login_player_pool_;
	Int_List drop_cid_list_;

	Tick_Info tick_info_;
	Time_Value tick_time_;

	int server_status_;

	/// 消息统计
	bool msg_count_onoff_;
	Msg_Count_Map inner_msg_count_map_;

	Ip_Vec gate_ip_vec_;
	Check_Account check_account_;

	Login_Player_Map player_cid_map_; /// cid - Login_Player map
	Login_Player_Account_Map player_account_map_; /// role_id - Login_Player map
};

#define LOGIN_MANAGER Login_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline Login_Player *Login_Manager::pop_login_player(void) {
	return login_player_pool_.pop();
}

inline int Login_Manager::push_login_player(Login_Player *player) {
	return login_player_pool_.push(player);
}

inline void Login_Manager::push_drop_cid(int cid) {
	drop_cid_list_.push_back(cid);
}

inline int Login_Manager::push_login_client_data(Block_Buffer *buf) {
	login_client_data_list_.push_back(buf);
	return 0;
}

inline int Login_Manager::push_login_gate_data(Block_Buffer *buf) {
	login_gate_data_list_.push_back(buf);
	return 0;
}

inline int Login_Manager::push_self_loop_message(Block_Buffer &msg_buf) {
	Block_Buffer *buf = block_pool_.pop();
	if (! buf) {
		LOG_INFO("block_pool_.pop return 0");
		return -1;
	}
	buf->reset();
	buf->copy(&msg_buf);
	self_loop_block_list_.push_back(buf);
	return 0;
}

inline void Login_Manager::set_msg_count_onoff(int v) {
	if (v == 0 || v == 1) {
		msg_count_onoff_ = v;
	} else {
		LOG_INFO("error value v = %d", v);
	}
}

inline void Login_Manager::inner_msg_count(Block_Buffer &buf) {
	/// len(int16), msg_id(int32), status(int32)
	if (msg_count_onoff_ && buf.readable_bytes() >= (sizeof(int16_t) + sizeof(int32_t) + sizeof(int32_t))) {
		uint16_t read_idx_orig = buf.get_read_idx();

		buf.set_read_idx(read_idx_orig + sizeof(int16_t));
		int32_t msg_id = buf.read_int32();
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);

		buf.set_read_idx(read_idx_orig);
	}
}

inline void Login_Manager::inner_msg_count(int msg_id) {
	if (msg_count_onoff_) {
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);
	}
}

inline Check_Account &Login_Manager::check_account(void) {
	return check_account_;
}

#endif /* LOGIN_MANAGER_H_ */
