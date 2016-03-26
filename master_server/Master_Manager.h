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

	//cid = gate_cid * 10000 + player_cid
	typedef boost::unordered_map<int, Master_Player* > Master_Player_Gate_Cid_Map;
	//cid = game_cid * 10000 + player_cid
	typedef boost::unordered_map<int, Master_Player* > Master_Player_Game_Cid_Map;
	typedef boost::unordered_map<int64_t, Master_Player* > Master_Player_Role_Id_Map;
	typedef boost::unordered_map<std::string, Master_Player* > Master_Player_Role_Name_Map;
	typedef boost::unordered_map<int, int> Msg_Count_Map;

public:
	enum {
		STATUS_NORMAL = 1,
		STATUS_CLOSING = 2,
	};

	static Master_Manager *instance(void);
	int init(void);
	void run_handler(void);

	/// 服务器状态
	inline int server_status(void) { return server_status_; }

	Master_Player *pop_master_player(void);
	int push_master_player(Master_Player *player);

	/// 发送数据接口
	int send_to_gate(int gate_cid, Block_Buffer &buf);
	int send_to_game(int game_cid, Block_Buffer &buf);
	/// 关闭客户端连接
	int close_client(int gate_cid, int player_cid, int error_code);
	/// 主动关闭处理
	int self_close_process(void);

	/// 广播消息给所有在线玩家
	int boardcast_msg_to_all(Block_Buffer &buf);

	/// 通信层投递消息到Master_Manager
	int push_master_gate_data(Block_Buffer *buf);
	int push_master_game_data(Block_Buffer *buf);
	int push_self_loop_message(Block_Buffer &msg_buf);

	/// 消息处理
	int process_list();

	//cid = gate_cid * 10000 + player_cid
	int bind_gate_cid_master_player(int cid, Master_Player &player);
	int unbind_gate_cid_master_player(int cid);
	Master_Player *find_gate_cid_master_player(int cid);

	//cid = game_cid * 10000 + player_cid
	int bind_game_cid_master_player(int cid, Master_Player &player);
	int unbind_game_cid_master_player(int cid);
	Master_Player *find_game_cid_master_player(int cid);

	int bind_role_id_master_player(int64_t role_id, Master_Player &player);
	int unbind_role_id_master_player(int64_t role_id);
	Master_Player *find_role_id_master_player(int64_t role_id);

	int bind_role_name_master_player(std::string& role_name, Master_Player &player);
	int unbind_role_name_master_player(std::string& role_name);
	Master_Player *find_role_name_master_player(std::string& role_name);

	int unbind_master_player(Master_Player &player);

	/// 定时器处理
	int tick(void);
	/// 返回上次tick的绝对时间, 最大误差有100毫秒,主要为减少系统调用gettimeofday()调用次数
	inline const Time_Value &tick_time(void) { return tick_time_; }
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

	Master_Player_Gate_Cid_Map player_gate_cid_map_;
	Master_Player_Gate_Cid_Map player_game_cid_map_;
	Master_Player_Role_Id_Map player_role_id_map_;
	Master_Player_Role_Name_Map player_role_name_map_;

	Tick_Info tick_info_;
	Time_Value tick_time_;

	int server_status_;

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
		LOG_INFO("block_pool_.pop return 0");
		return -1;
	}
	buf->reset();
	buf->copy(&msg_buf);
	self_loop_block_list_.push_back(buf);
	return 0;
}

inline void Master_Manager::set_msg_count_onoff(int v) {
	if (v == 0 || v == 1) {
		msg_count_onoff_ = v;
	} else {
		LOG_INFO("error value v = %d", v);
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
