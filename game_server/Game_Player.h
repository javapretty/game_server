/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_

#include "Public_Struct.h"

class Game_Player {
public:
	const static Time_Value save_interval_;

	Game_Player(void);
	virtual ~Game_Player(void);

	int respond_success_result(int msg_id, Block_Buffer *buf = 0);
	int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);

	inline void set_cid(int gate_cid, int player_cid) {
		gate_cid_ = gate_cid;
		player_cid_ = player_cid;
	}
	inline int gate_cid(void) { return gate_cid_; }
	inline int player_cid(void) { return player_cid_; }
	inline Player_Data &player_data(void) { return player_data_; }
	inline Block_Buffer *write_player_data_buffer(void) { return write_player_data_buffer_; }

	int load_player(Player_Data &player_data);
	int save_player(bool is_logout = false);

	int sign_in(std::string account);
	int sign_out(void);
	void reset(void);

	int sync_signin_to_master(void);
	int sync_signout_to_master(void);
	int respond_role_login(void);

	int tick(Time_Value &now);
	int link_close(void);
	int recycle_status(void) { return recycle_tick_.status; }

private:
	int gate_cid_;			//gate连接game的cid
	int player_cid_;		//player连接gate的cid
	Player_Data player_data_;
	Block_Buffer *read_player_data_buffer_;			//供js端读取登录数据的buffer
	Block_Buffer *write_player_data_buffer_;		//供js端写入玩家数据的buffer
	Recycle_Tick recycle_tick_;
	Time_Value last_save_tick_;			//上次保存数据tick
};

#endif /* GAME_PLAYER_H_ */
