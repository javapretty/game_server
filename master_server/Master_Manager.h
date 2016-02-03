/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef MASTER_MANAGER_H_
#define MASTER_MANAGER_H_

#include "Log.h"
#include "Block_Buffer.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Master_Player.h"


class Master_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Object_Pool<Master_Player, Spin_Lock> Master_Player_Pool;
	typedef Block_List<Thread_Mutex> Data_List;

	typedef boost::unordered_map<role_id_t, Master_Player *> Master_Player_Role_Id_Map;
	typedef boost::unordered_map<int, int> Msg_Count_Map;

public:
	enum {
		STATUS_NORMAL = 1,
		STATUS_CLOSING = 2,
	};

	static Master_Manager *instance(void);
	int init(void);
	void run_handler(void);

	/// 定时器处理
	int register_timer(void);
	int unregister_timer(void);
	int time_up(const Time_Value &now);

	Master_Player *pop_master_player(void);
	int push_master_player(Master_Player *player);

	/// 发送数据接口
	int send_to_gate(int cid, Block_Buffer &buf);
	int send_to_game(int cid, Block_Buffer &buf);

	/// 服务器状态
	int server_status(void);

	/// 通信层投递消息到Master_Manager
	int push_master_gate_data(Block_Buffer *buf);
	int push_master_game_data(Block_Buffer *buf);
	int push_self_loop_message(Block_Buffer &msg_buf);

	/// 消息处理
	int process_list();

	int bind_role_id_master_player(role_id_t role_id, Master_Player &player);
	int unbind_role_id_master_player(role_id_t role_id);
	Master_Player *find_role_id_master_player(role_id_t role_id);
	int unbind_master_player(Master_Player &player);

	int tick(void);
	int server_info_tick(Time_Value &now);
	int player_tick(Time_Value &now);
	int manager_tick(Time_Value &now);
	void object_pool_tick(Time_Value &now);

	inline Master_Player_Role_Id_Map& player_role_id_map(void);
	void get_server_info(Block_Buffer &buf);

	/// 返回上次tick的绝对时间, 最大误差有100毫秒
	/// 主要为减少系统调用gettimeofday()调用次数
	const Time_Value &tick_time(void);
	void object_pool_size(void);
	void free_cache(void);

	/// 统计内部消息量
	void set_msg_count_onoff(int v);
	void print_msg_count(void);
	void inner_msg_count(Block_Buffer &buf);
	void inner_msg_count(int msg_id);

private:
	Master_Manager(void);
	virtual ~Master_Manager(void);
	Master_Manager(const Master_Manager &);
	const Master_Manager &operator=(const Master_Manager &);

private:
	static Master_Manager *instance_;

	Block_Pool block_pool_;
	Master_Player_Pool master_player_pool_;

	Data_List master_gate_data_list_;			//gate-->master
	Data_List master_game_data_list_;			//game-->master
	Data_List self_loop_block_list_; 			//self_loop_block_list

	Server_Info master_gate_server_info_;
	Server_Info master_game_server_info_;

	Master_Player_Role_Id_Map player_role_id_map_; /// role_id - Master_Player map

	Tick_Info tick_info_;

	int status_;
	bool is_register_timer_;
	Time_Value tick_time_;

	/// 消息统计
	bool msg_count_onoff_;
	Msg_Count_Map inner_msg_count_map_; //内部消息统计
};

#define MASTER_MANAGER Master_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline Master_Player *Master_Manager::pop_master_player(void) {
	return master_player_pool_.pop();
}

inline int Master_Manager::push_master_player(Master_Player *player) {
	return master_player_pool_.push(player);
}

inline Master_Manager::Master_Player_Role_Id_Map& Master_Manager::player_role_id_map(void) {
	return player_role_id_map_;
}

inline int Master_Manager::push_master_gate_data(Block_Buffer *buf) {
	master_gate_data_list_.push_back(buf);
	return 0;
}

inline int Master_Manager::push_master_game_data(Block_Buffer *buf) {
	master_game_data_list_.push_back(buf);
	return 0;
}

inline int Master_Manager::push_self_loop_message(Block_Buffer &msg_buf) {
	Block_Buffer *buf = block_pool_.pop();
	if (! buf) {
		MSG_USER("block_pool_.pop return 0");
		return -1;
	}
	buf->reset();
	buf->copy(&msg_buf);
	self_loop_block_list_.push_back(buf);
	return 0;
}

inline const Time_Value &Master_Manager::tick_time(void) {
	return tick_time_;
}

inline void Master_Manager::set_msg_count_onoff(int v) {
	if (v == 0 || v == 1) {
		msg_count_onoff_ = v;
	} else {
		MSG_USER("error value v = %d", v);
	}
}

inline void Master_Manager::inner_msg_count(Block_Buffer &buf) {
	/// len(int16), msg_id(int32), status(int32)
	if (msg_count_onoff_ && buf.readable_bytes() >= (sizeof(int16_t) + sizeof(int32_t) + sizeof(int32_t))) {
		uint16_t read_idx_orig = buf.get_read_idx();

		buf.set_read_idx(read_idx_orig + sizeof(int16_t));
		int32_t msg_id = buf.read_int32();
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);

		buf.set_read_idx(read_idx_orig);
	}
}

inline void Master_Manager::inner_msg_count(int msg_id) {
	if (msg_count_onoff_) {
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);
	}
}

#endif /* MASTER_MANAGER_H_ */
