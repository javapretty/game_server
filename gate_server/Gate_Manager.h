/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GATE_MANAGER_H_
#define GATE_MANAGER_H_

#include "Log.h"
#include "Block_Buffer.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Gate_Player.h"


class Gate_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Object_Pool<Gate_Player, Spin_Lock> Gate_Player_Pool;

	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;
	typedef std::list<Close_Info> Close_List;

	typedef boost::unordered_map<int, Gate_Player *> Gate_Player_Cid_Map;
	typedef boost::unordered_map<std::string, Gate_Player *> Gate_Player_Account_Map;
	typedef boost::unordered_map<int, int> Msg_Count_Map;

public:
	enum {
		STATUS_NORMAL = 1,
		STATUS_CLOSING = 2,
	};

	static Gate_Manager *instance(void);
	int init(void);
	void run_handler(void);

	/// 服务器状态
	inline int server_status(void) { return server_status_; };

	Gate_Player *pop_gate_player(void);
	int push_gate_player(Gate_Player *player);

	/// 发送数据接口
	int send_to_client(int player_cid, Block_Buffer &buf);
	int send_to_game(Block_Buffer &buf);
	int send_to_login(Block_Buffer &buf);
	int send_to_master(Block_Buffer &buf);
	/// 关闭客户端连接
	int close_client(int player_cid);
	/// 主动关闭处理
	int self_close_process(void);

	/// 通信层投递消息到Login_Manager
	void push_drop_cid(int cid);
	int push_gate_client_data(Block_Buffer *buf);
	int push_gate_login_data(Block_Buffer *buf);
	int push_gate_game_data(Block_Buffer *buf);
	int push_gate_master_data(Block_Buffer *buf);
	int push_self_loop_message(Block_Buffer &msg_buf);

	/// 消息处理
	int process_list();
	void process_drop_cid(int cid);

	//////////////////// Player Pool and Map Container Operator ////////////////////
	int bind_cid_gate_player(int cid, Gate_Player &player);
	int unbind_cid_gate_player(int cid);
	Gate_Player *find_cid_gate_player(int cid);

	int bind_account_gate_player(std::string& account, Gate_Player &player);
	int unbind_account_gate_player(std::string& account);
	Gate_Player *find_account_gate_player(std::string& account);

	int unbind_gate_player(Gate_Player &player);

	/// 定时器处理
	int tick(void);
	/// 返回上次tick的绝对时间, 最大误差有100毫秒,主要为减少系统调用gettimeofday()调用次数
	inline const Time_Value &tick_time(void) { return tick_time_; };
	int close_list_tick(Time_Value &now);
	int server_info_tick(Time_Value &now);
	int player_tick(Time_Value &now);
	void object_pool_tick(Time_Value &now);

	void get_server_info(Block_Buffer &buf);
	void get_server_ip_port(int player_cid, std::string &ip, int &port);
	void object_pool_size(void);
	void free_cache(void);

	std::string &md5_key(void) { return md5_key_; };
	bool verify_pack(void) { return verify_pack_onoff_; };

	/// 统计内部消息量
	void set_msg_count_onoff(int v);
	void print_msg_count(void);
	void inner_msg_count(Block_Buffer &buf);
	void inner_msg_count(int msg_id);

private:
	Gate_Manager(void);
	virtual ~Gate_Manager(void);
	Gate_Manager(const Gate_Manager &);
	const Gate_Manager &operator=(const Gate_Manager &);

private:
	static Gate_Manager *instance_;

	Block_Pool block_pool_;
	Gate_Player_Pool gate_player_pool_;

	Int_List drop_cid_list_;
	Data_List gate_client_data_list_;				///client-->gate
	Data_List gate_login_data_list_;					///login-->gate
	Data_List gate_game_data_list_;					///game-->gate
	Data_List gate_master_data_list_;				///master-->gate
	Data_List self_loop_block_list_; 				/// self_loop_block_list
	Close_List close_list_; 								/// 其中的连接cid在n秒后投递到通信层关闭

	Server_Info gate_client_server_info_;

	Gate_Player_Cid_Map player_cid_map_; /// cid - Login_Player map
	Gate_Player_Account_Map player_account_map_;

	Tick_Info tick_info_;
	Time_Value tick_time_;

	int server_status_;

	std::string md5_key_;

	bool verify_pack_onoff_;
	/// 消息统计
	bool msg_count_onoff_;
	Msg_Count_Map inner_msg_count_map_; //内部消息统计
};

#define GATE_MANAGER Gate_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline Gate_Player *Gate_Manager::pop_gate_player(void) {
	return gate_player_pool_.pop();
}

inline int Gate_Manager::push_gate_player(Gate_Player *player) {
	return gate_player_pool_.push(player);
}

inline void Gate_Manager::push_drop_cid(int cid) {
	drop_cid_list_.push_back(cid);
}

inline int Gate_Manager::push_gate_client_data(Block_Buffer *buf) {
	gate_client_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_gate_login_data(Block_Buffer *buf) {
	gate_login_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_gate_game_data(Block_Buffer *buf) {
	gate_game_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_gate_master_data(Block_Buffer *buf) {
	gate_master_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_self_loop_message(Block_Buffer &msg_buf) {
	Block_Buffer *buf = block_pool_.pop();
	if (! buf) {
		return -1;
	}
	buf->reset();
	buf->copy(&msg_buf);
	self_loop_block_list_.push_back(buf);
	return 0;
}

inline void Gate_Manager::set_msg_count_onoff(int v) {
	if (v == 0 || v == 1) {
		msg_count_onoff_ = v;
	}
}

inline void Gate_Manager::inner_msg_count(Block_Buffer &buf) {
	/// len(int16), msg_id(int32), status(int32)
	if (msg_count_onoff_ && buf.readable_bytes() >= (sizeof(int16_t) + sizeof(int32_t) + sizeof(int32_t))) {
		uint16_t read_idx_orig = buf.get_read_idx();

		buf.set_read_idx(read_idx_orig + sizeof(int16_t));
		int32_t msg_id = buf.read_int32();
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);

		buf.set_read_idx(read_idx_orig);
	}
}

inline void Gate_Manager::inner_msg_count(int msg_id) {
	if (msg_count_onoff_) {
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);
	}
}

#endif /* GATE_MANAGER_H_ */
