/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_

#include "Public_Struct.h"

class Game_Player {
public:
	Game_Player(void);
	virtual ~Game_Player(void);

	int respond_success_result(int msg_id, Block_Buffer *buf = 0);
	int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);

	inline void set_gate_cid(int gate_cid, int player_cid) {
		gate_cid_ = gate_cid;
		player_cid_ = player_cid;
	}
	inline int gate_cid(void) { return gate_cid_; }
	inline int player_cid(void) { return player_cid_; }

	void reset(void);
	int tick(Time_Value &now);
	int link_close(void);

private:
	int gate_cid_;			//gate连接game的cid
	int player_cid_;		//player连接gate的cid
	Recycle_Tick recycle_tick_;
};

#endif /* GAME_PLAYER_H_ */
