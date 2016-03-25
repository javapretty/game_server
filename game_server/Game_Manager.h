/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Log.h"
#include "Block_Buffer.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Game_Player.h"
#include "Msg_Define.h"

class Game_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Object_Pool<Game_Player, Spin_Lock> Game_Player_Pool;
	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;
	typedef boost::unordered_map<std::string, Cid_Info> Logining_Map;
	typedef boost::unordered_map<int64_t, Saving_Info> Saving_Map;

	//cid = gate_cid * 10000 + player_cid
	typedef boost::unordered_map<int, Game_Player* > Game_Player_Cid_Map;
	typedef boost::unordered_map<int64_t, Game_Player* > Game_Player_Role_Id_Map;
	typedef boost::unordered_map<std::string, Game_Player* > Game_Player_Role_Name_Map;
	typedef boost::unordered_map<int, int> Msg_Count_Map;

public:
	const static int32_t saving_player_time;

	enum {
		STATUS_NORMAL = 1,
		STATUS_CLOSING = 2,
	};

	static Game_Manager *instance(void);
	int init(void);
	void run_handler(void);

	int load_db_cache(void);
	inline DB_Cache *db_cache(void) { return db_cache_; }

	/// 服务器状态
	inline int server_status(void) { return server_status_; };

	Game_Player *pop_game_player(void);
	int push_game_player(Game_Player *player);
	Block_Buffer *pop_block_buffer(void);
	int push_block_buffer(Block_Buffer *buf);

	/// 发送数据接口
	int send_to_gate(int cid, Block_Buffer &buf);
	int send_to_master(Block_Buffer &buf);
	int send_to_db(Block_Buffer &buf);

	/// 通信层投递消息到Game_Manager
	void push_drop_gate_cid(int cid);
	int push_game_gate_data(Block_Buffer *buf);
	Block_Buffer* pop_game_client_data();
	int push_game_db_data(Block_Buffer *buf);
	int push_game_master_data(Block_Buffer *buf);
	int push_self_loop_message(Block_Buffer &msg_buf);

	int push_player_data(Block_Buffer *buf);
	Block_Buffer* pop_player_data(void);
	int push_drop_player_cid(int cid);
	int pop_drop_player_cid(void);
	/// 消息处理
	int process_list();
	void process_drop_gate_cid(int gate_cid);

	//cid = gate_cid * 10000 + player_cid
	int bind_cid_game_player(int cid, Game_Player &player);
	int unbind_cid_game_player(int cid);
	Game_Player* find_cid_game_player(int cid);

	//通过role_id定位玩家
	int bind_role_id_game_player(int64_t role_id, Game_Player &player);
	int unbind_role_id_game_player(int64_t role_id);
	Game_Player* find_role_id_game_player(int64_t role_id);

	//通过role_name定位玩家
	int bind_role_name_game_player(std::string &role_name, Game_Player &player);
	int unbind_role_name_game_player(std::string &role_name);
	Game_Player* find_role_name_game_player(std::string &role_name);

	int unbind_game_player(Game_Player &player);

	Logining_Map &logining_map(void);
	Saving_Map &saving_map(void);

	/// 定时器处理
	int tick(void);
	/// 返回上次tick的绝对时间, 最大误差有100毫秒,主要为减少系统调用gettimeofday()调用次数
	inline const Time_Value &tick_time(void) { return tick_time_; };
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
	DB_Cache *db_cache_;

	Block_Pool block_pool_;
	Game_Player_Pool game_player_pool_;

	Data_List loaded_player_data_list_; 			//玩家登录从数据库加载数据
	Int_List drop_gate_cid_list_;						//掉线gate_cid列表,让通过该gate连接到game的所有玩家掉线
	Data_List player_login_data_list_;				//玩家登录消息数据
	Data_List game_client_data_list_;				//玩家逻辑消息数据，发到js处理
	Data_List game_db_data_list_;						///db-->game
	Data_List game_master_data_list_;				///master-->game
	Data_List self_loop_block_list_; 				///self_loop_block_list

	Data_List player_data_list_; 						//玩家数据,传送给js层
	Int_List drop_player_cid_list_;					//掉线的玩家cid列表

	Server_Info game_gate_server_info_;

	Logining_Map logining_map_; /// 正在上线加载流程中的帐号
	Saving_Map saving_map_; /// 正在下线保存流程中的帐号

	Game_Player_Cid_Map player_cid_map_; 							/// cid_info 	- Game_Player
	Game_Player_Role_Id_Map player_role_id_map_; 			/// role_id 	- Game_Player
	Game_Player_Role_Name_Map player_role_name_map_;	/// role_name - Game_Player

	Tick_Info tick_info_;
	Time_Value tick_time_;

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

inline Game_Manager::Logining_Map &Game_Manager::logining_map(void) {
	return logining_map_;
}

inline Game_Manager::Saving_Map &Game_Manager::saving_map(void) {
	return saving_map_;
}

inline void Game_Manager::push_drop_gate_cid(int cid) {
	drop_gate_cid_list_.push_back(cid);
}

inline int Game_Manager::push_game_gate_data(Block_Buffer *buf) {
	int read_idx = buf->get_read_idx();
	/*int32_t cid*/ buf->read_int32();
	/*int16_t len*/ buf->read_int16();
	int32_t msg_id = buf->read_int32();
	buf->set_read_idx(read_idx);

	switch (msg_id) {
	case REQ_FETCH_ROLE_INFO:
	case REQ_CREATE_ROLE:
	case SYNC_GATE_GAME_PLAYER_SIGNOUT: {
		player_login_data_list_.push_back(buf);
		break;
	}
	default : {
		game_client_data_list_.push_back(buf);
	}
	}
	return 0;
}

inline Block_Buffer* Game_Manager::pop_game_client_data() {
	return game_client_data_list_.pop_front();
}

inline int Game_Manager::push_game_db_data(Block_Buffer *buf) {
	int read_idx = buf->get_read_idx();
	/*int32_t cid*/ buf->read_int32();
	/*int16_t len*/ buf->read_int16();
	int32_t msg_id = buf->read_int32();
	buf->set_read_idx(read_idx);

	switch (msg_id) {
	case SYNC_DB_GAME_LOAD_PLAYER_INFO:
	case SYNC_DB_GAME_CREATE_PLAYER: {
		loaded_player_data_list_.push_back(buf);
		break;
	}
	default : {
		game_db_data_list_.push_back(buf);
	}
	}
	return 0;
}

inline int Game_Manager::push_game_master_data(Block_Buffer *buf) {
	game_master_data_list_.push_back(buf);
	return 0;
}

inline int Game_Manager::push_self_loop_message(Block_Buffer &msg_buf) {
	Block_Buffer *buf = pop_block_buffer();
	if (!buf) {
		return -1;
	}
	buf->reset();
	buf->copy(&msg_buf);
	self_loop_block_list_.push_back(buf);
	return 0;
}

inline int Game_Manager::push_player_data(Block_Buffer *buf) {
	player_data_list_.push_back(buf);
	return 0;
}

inline Block_Buffer* Game_Manager::pop_player_data(void) {
	return player_data_list_.pop_front();
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
		LOG_INFO("error value v = %d", v);
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
