/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Msg_Define.h"
#include "Public_Struct.h"
#include "Public_Define.h"
#include "Game_Timer.h"
#include "Block_Buffer.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"
#include "Object_Pool.h"
#include "Log.h"

class Game_Player;

class Game_Manager: public Thread {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef Object_Pool<Game_Player, Spin_Lock> Game_Player_Pool;
	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;

	typedef boost::unordered_map<std::string, Cid_Info> Logining_Map;
	typedef boost::unordered_map<role_id_t, Saving_Info> Saving_Map;

	//通过某个gate连接到game的玩家放到同map内,该map用player_cid和gate_cid作为key
	typedef boost::unordered_map<Cid_Info, Game_Player* > Game_Player_Cid_Map;
	typedef boost::unordered_map<role_id_t, Game_Player* > Game_Player_Role_Id_Map;
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
	DB_Cache *db_cache(void) { return db_cache_; }

	Game_Player *pop_game_player(void);
	int push_game_player(Game_Player *player);

	/// 发送数据接口
	int send_to_gate(int cid, Block_Buffer &buf);
	int send_to_master(Block_Buffer &buf);
	int send_to_db(Block_Buffer &buf);

	/// 服务器状态
	int server_status(void);

	/// 通信层投递消息到Game_Manager
	void push_drop_gate_cid(int cid);
	int push_game_gate_data(Block_Buffer *buf);
	int push_game_db_data(Block_Buffer *buf);
	int push_game_master_data(Block_Buffer *buf);
	int push_self_loop_message(Block_Buffer &msg_buf);

	/// 消息处理
	int process_list();
	void process_drop_gate_cid(int gate_cid);

	//通过player_cid和gate_cid双重定位到玩家
	int bind_cid_game_player(Cid_Info &cid_info, Game_Player &player);
	int unbind_cid_game_player(Cid_Info &cid_info);
	Game_Player* find_cid_game_player(Cid_Info &cid_info);

	//通过role_id定位玩家
	int bind_role_id_game_player(role_id_t role_id, Game_Player &player);
	int unbind_role_id_game_player(role_id_t role_id);
	Game_Player* find_role_id_game_player(role_id_t role_id);

	//通过role_name定位玩家
	int bind_role_name_game_player(std::string &role_name, Game_Player &player);
	int unbind_role_name_game_player(std::string &role_name);
	Game_Player* find_role_name_game_player(std::string &role_name);

	int unbind_game_player(Game_Player &player);

	Logining_Map &logining_map(void);
	Saving_Map &saving_map(void);

	/// 定时器处理
	int register_timer(void);
	int unregister_timer(void);
	int time_up(const Time_Value &now);

	int tick(void);
	int server_info_tick(Time_Value &now);
	int player_tick(Time_Value &now);
	int manager_tick(Time_Value &now);
	int saving_scanner_tick(Time_Value &now);
	void object_pool_tick(Time_Value &now);

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
	Data_List game_gate_data_list_;					///gate-->game
	Data_List game_db_data_list_;						///db-->game
	Data_List game_master_data_list_;				///master-->game
	Data_List self_loop_block_list_; 				///self_loop_block_list

	Server_Info game_gate_server_info_;

	Logining_Map logining_map_; /// 正在上线加载流程中的帐号
	Saving_Map saving_map_; /// 正在下线保存流程中的帐号

	Game_Player_Cid_Map player_cid_map_; 							/// cid_info 	- Game_Player
	Game_Player_Role_Id_Map player_role_id_map_; 			/// role_id 	- Game_Player
	Game_Player_Role_Name_Map player_role_name_map_;	/// role_name - Game_Player

	Tick_Info tick_info_;

	int status_;
	bool is_register_timer_;
	Time_Value tick_time_;

	/// 消息统计
	bool msg_count_onoff_;
	Msg_Count_Map inner_msg_count_map_; //内部消息统计
};

#define GAME_MANAGER Game_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
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
	game_gate_data_list_.push_back(buf);
	return 0;
}

inline int Game_Manager::push_game_db_data(Block_Buffer *buf) {
	int32_t cid = 0;
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t status = 0;

	int read_idx = buf->get_read_idx();
	buf->read_int32(cid);
	buf->read_uint16(len);
	buf->read_uint32(msg_id);
	buf->read_int32(status);
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

inline const Time_Value &Game_Manager::tick_time(void) {
	return tick_time_;
}

inline void Game_Manager::set_msg_count_onoff(int v) {
	if (v == 0 || v == 1) {
		msg_count_onoff_ = v;
	} else {
		MSG_USER("error value v = %d", v);
	}
}

inline void Game_Manager::inner_msg_count(Block_Buffer &buf) {
	/// len(uint16), msg_id(uint32), status(uint32)
	if (msg_count_onoff_ && buf.readable_bytes() >= (sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t))) {
		uint16_t read_idx_orig = buf.get_read_idx();

		buf.set_read_idx(read_idx_orig + sizeof(uint16_t));
		uint32_t msg_id = 0;
		buf.read_uint32(msg_id);
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
