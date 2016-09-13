/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Server_Manager.h"
#include "Game_Player.h"

class Game_Manager: public Server_Manager {
	typedef Object_Pool<Game_Player, Spin_Lock> Player_Pool;
public:
	static Game_Manager *instance(void);
	int init(int server_id);

	inline Game_Player *pop_player(void) { return player_pool_.pop(); }
	inline int push_player(Game_Player *player) { return  player_pool_.push(player); }

	virtual int close_client(int gate_cid, int player_cid, int error_code);
	virtual int recycle_player(int gate_cid, int player_cid);
	virtual int free_cache(void);
	virtual void get_server_info(void);
	virtual void print_server_info(void);

	//发送数据
	int send_to_gate(int gate_cid, Block_Buffer &buf);
	int send_to_master(Block_Buffer &buf);
	int send_to_db(Block_Buffer &buf);
	int send_to_log(Block_Buffer &buf);

	//消息处理
	int process_list();
	int process_drop_gate_cid(int gate_cid);
	int push_drop_gate_cid(int gate_cid);
	int push_tick(int x);
	int push_game_gate_data(Block_Buffer *buf);
	Block_Buffer* pop_game_gate_data();
	int push_game_db_data(Block_Buffer *buf);
	Block_Buffer* pop_game_db_data();
	int push_game_master_data(Block_Buffer *buf);
	Block_Buffer* pop_game_master_data(void);
	int push_drop_player_cid(int cid);
	int pop_drop_player_cid(void);

private:
	Game_Manager(void);
	virtual ~Game_Manager(void);
	Game_Manager(const Game_Manager &);
	const Game_Manager &operator=(const Game_Manager &);

private:
	static Game_Manager *instance_;

	Player_Pool player_pool_;

	Int_List drop_gate_cid_list_;				//掉线gate_cid列表，让通过该gate连接到game的所有玩家掉线
	Int_List tick_list_;									//定时器列表
	Data_List game_gate_data_list_;			//gate--game
	Data_List game_db_data_list_;				//db-->game
	Data_List game_master_data_list_;		//master-->game
	Int_List drop_player_cid_list_;			//掉线的玩家cid列表

	Server_Info game_gate_server_info_;
	Server_Info game_db_connector_info_;
	Server_Info game_master_connector_info_;
};

#define GAME_MANAGER Game_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline int Game_Manager::push_drop_gate_cid(int gate_cid) {
	drop_gate_cid_list_.push_back(gate_cid);
	return 0;
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

#endif /* GAME_MANAGER_H_ */
