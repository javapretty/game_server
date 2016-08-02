/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Log.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Game_Player.h"

class Game_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Object_Pool<Game_Player, Spin_Lock> Game_Player_Pool;
	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;

	//cid = gate_cid * 10000 + player_cid
	typedef boost::unordered_map<int, Game_Player* > Game_Player_Cid_Map;
	typedef boost::unordered_map<int, int> Msg_Count_Map;

public:
	enum {
		STATUS_NORMAL = 1,
		STATUS_CLOSING = 2,
	};

	static Game_Manager *instance(void);
	int init(void);
	void run_handler(void);

	/// 服务器状态
	inline int server_status(void) { return server_status_; }

	Game_Player *pop_game_player(void);
	int push_game_player(Game_Player *player);
	Block_Buffer *pop_block_buffer(void);
	int push_block_buffer(Block_Buffer *buf);

	/// 发送数据接口
	int send_to_gate(int gate_cid, Block_Buffer &buf);
	int send_to_master(Block_Buffer &buf);
	int send_to_db(Block_Buffer &buf);
	int send_to_log(Block_Buffer &buf);

	/// 关闭客户端连接
	int close_client(int gate_cid, int player_cid, int error_code);
	/// 主动关闭处理
	int self_close_process(void);

	/// 消息处理
	int process_list();
	void process_drop_gate_cid(int gate_cid);

	/// 通信层投递消息到Game_Manager
	void push_drop_gate_cid(int cid);
	int push_tick(int x);
	int push_game_gate_data(Block_Buffer *buf);
	Block_Buffer* pop_game_gate_data();
	int push_game_db_data(Block_Buffer *buf);
	Block_Buffer* pop_game_db_data();
	int push_game_master_data(Block_Buffer *buf);
	Block_Buffer* pop_game_master_data(void);
	int push_drop_player_cid(int cid);
	int pop_drop_player_cid(void);

	//cid = gate_cid * 10000 + player_cid
	int bind_cid_game_player(int cid, Game_Player &player);
	int unbind_cid_game_player(int cid);
	Game_Player* find_cid_game_player(int cid);
	int unbind_game_player(Game_Player &player);

	/// 定时器处理
	int tick(void);
	int player_tick(Time_Value &now);
	int server_info_tick(Time_Value &now);
	int saving_scanner_tick(Time_Value &now);
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
	Game_Manager(void);
	virtual ~Game_Manager(void);
	Game_Manager(const Game_Manager &);
	const Game_Manager &operator=(const Game_Manager &);

private:
	static Game_Manager *instance_;

	Block_Pool block_pool_;
	Game_Player_Pool game_player_pool_;

	Int_List drop_gate_cid_list_;				//掉线gate_cid列表,让通过该gate连接到game的所有玩家掉线
	Int_List tick_list_;								//定时器列表
	Data_List game_gate_data_list_;			//gate--game
	Data_List game_db_data_list_;				//db-->game
	Data_List game_master_data_list_;		//master-->game
	Int_List drop_player_cid_list_;			//掉线的玩家cid列表

	Game_Player_Cid_Map player_cid_map_; 	//cid_info--Game_Player

	Server_Info game_gate_server_info_;
	Tick_Info tick_info_;

	int server_status_;

	/// 消息统计
	bool msg_count_onoff_;
	Msg_Count_Map inner_msg_count_map_; //内部消息统计
};

#define GAME_MANAGER Game_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline Game_Player *Game_Manager::pop_game_player(void) {
	return game_player_pool_.pop();
}

inline int Game_Manager::push_game_player(Game_Player *player) {
	return game_player_pool_.push(player);
}

inline Block_Buffer *Game_Manager::pop_block_buffer(void) {
	return block_pool_.pop();
}

inline int Game_Manager::push_block_buffer(Block_Buffer *buf) {
	return block_pool_.push(buf);
}

inline void Game_Manager::push_drop_gate_cid(int cid) {
	drop_gate_cid_list_.push_back(cid);
}

inline int Game_Manager::push_tick(int x) {
	tick_list_.push_back(x);
	return 0;
}

inline int Game_Manager::push_game_gate_data(Block_Buffer *buf) {
	return game_gate_data_list_.push_back(buf);
}

inline Block_Buffer* Game_Manager::pop_game_gate_data() {
	return game_gate_data_list_.pop_front();
}

inline int Game_Manager::push_game_db_data(Block_Buffer *buf) {
	return game_db_data_list_.push_back(buf);
}

inline Block_Buffer* Game_Manager::pop_game_db_data() {
	return game_db_data_list_.pop_front();
}

inline int Game_Manager::push_game_master_data(Block_Buffer *buf) {
	game_master_data_list_.push_back(buf);
	return 0;
}

inline Block_Buffer* Game_Manager::pop_game_master_data(void){
	return game_master_data_list_.pop_front();
}

inline int Game_Manager::push_drop_player_cid(int cid) {
	drop_player_cid_list_.push_back(cid);
	return 0;
}

inline int Game_Manager::pop_drop_player_cid(void) {
	if (drop_player_cid_list_.empty()) {
		return 0;
	}
	return drop_player_cid_list_.pop_front();
}

inline void Game_Manager::set_msg_count_onoff(int v) {
	if (v == 0 || v == 1) {
		msg_count_onoff_ = v;
	} else {
		LOG_ERROR("error value v = %d", v);
	}
}

inline void Game_Manager::inner_msg_count(Block_Buffer &buf) {
	/// len(int16), msg_id(int32), status(int32)
	if (msg_count_onoff_ && buf.readable_bytes() >= (sizeof(int16_t) + sizeof(int32_t) + sizeof(int32_t))) {
		uint16_t read_idx_orig = buf.get_read_idx();

		buf.set_read_idx(read_idx_orig + sizeof(int16_t));
		int32_t msg_id = buf.read_int32();
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);

		buf.set_read_idx(read_idx_orig);
	}
}

inline void Game_Manager::inner_msg_count(int msg_id) {
	if (msg_count_onoff_) {
		++(inner_msg_count_map_[static_cast<int>(msg_id)]);
	}
}

#endif /* GAME_MANAGER_H_ */
