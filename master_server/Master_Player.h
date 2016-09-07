/*
 *  Created on: Dec 21, 2015
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

	inline void set_gate_cid(int gate_cid, int player_cid) {
		gate_cid_ = gate_cid;
		player_cid_ = player_cid;
	}
	inline void set_game_cid(int game_cid, int player_cid) {
		game_cid_ = game_cid;
		player_cid_ = player_cid;
	}
	inline void set_role_id(int64_t role_id) { role_id_ = role_id; }
	inline int gate_cid(void) { return gate_cid_; }
	inline int game_cid(void) { return game_cid_; }
	inline int player_cid(void) { return player_cid_; }
	inline int64_t role_id(void) { return role_id_; }

	void reset(void);
	int tick(Time_Value &now);
	int link_close(void);

private:
	int gate_cid_;				//gate连接game的cid
	int game_cid_;				//game连接master的cid
	int player_cid_;			//player连接gate的cid
	int64_t role_id_;			//player role_id
	Recycle_Tick recycle_tick_;
};

#endif /* MASTER_PLAYER_H_ */
