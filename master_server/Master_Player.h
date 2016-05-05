/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef MASTER_PLAYER_H_
#define MASTER_PLAYER_H_

#include "Public_Struct.h"

class Master_Player {
public:
	Master_Player(void);
	virtual ~Master_Player(void);

	int respond_success_result(int msg_id, Block_Buffer *buf = 0);
	int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);

	inline void set_gate_cid(int gate_cid) { gate_cid_ = gate_cid; }
	inline int gate_cid(void) { return gate_cid_; }
	inline void set_game_cid(int game_cid) { game_cid_ = game_cid; }
	inline int game_cid(void) { return game_cid_; }
	inline void set_player_cid(int player_cid) { player_cid_ = player_cid; }
	inline int player_cid(void) { return player_cid_; }
	inline Master_Player_Info const &master_player_info(void) const { return player_info_; }
	inline Block_Buffer *save_player_data_buffer(void) { return save_player_data_buffer_; }

	int load_player(Master_Player_Info &player_info);
	int save_player(bool is_logout = false);

	int sign_in(void);
	int sign_out(void);
	void reset(void);

	int tick(Time_Value &now);
	int link_close(void);

private:
	int gate_cid_;				//gate连接game的cid
	int game_cid_;				//game连接master的cid
	int player_cid_;			//player连接gate的cid
	Master_Player_Info player_info_;
	Block_Buffer *load_player_data_buffer_;			//供js端读取登录数据的buffer
	Block_Buffer *save_player_data_buffer_;			//供js端写入玩家数据的buffer
	Recycle_Tick recycle_tick_;
};

#endif /* MASTER_PLAYER_H_ */
