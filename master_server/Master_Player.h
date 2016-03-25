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

	inline void set_gate_cid(int gate_cid) { gate_cid_ = gate_cid; };
	inline int gate_cid(void) { return gate_cid_; }
	inline void set_game_cid(int game_cid) { game_cid_ = game_cid; };
	inline int game_cid(void) { return game_cid_; }
	inline void set_player_cid(int player_cid) { player_cid_ = player_cid; };
	inline int player_cid(void) { return player_cid_; }
	inline void set_master_player_info(const Master_Player_Info &player_info) { player_info_ = player_info; };
	Master_Player_Info const &master_player_info(void) const { return player_info_; };

	int sign_in(Master_Player_Info &player_info);
	int sign_out(void);
	void reset(void);

	int tick(Time_Value &now);
	void set_recycle(void);
	int recycle_status(void);
	int recycle_tick(const Time_Value &now);
	int link_close(void);

	//聊天功能
	int send_chat_info(Block_Buffer &buf);

private:
	int gate_cid_;				//gate连接game的cid
	int game_cid_;				//game连接master的cid
	int player_cid_;			//player连接gate的cid
	Master_Player_Info player_info_;
	Recycle_Tick recycle_tick_;
};

////////////////////////////////////////////////////////////////////////////////
inline int Master_Player::link_close() {
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

inline void Master_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);
}

inline int Master_Player::recycle_status(void) {
	return recycle_tick_.status_;
}

#endif /* MASTER_PLAYER_H_ */
