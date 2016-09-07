/*
 * Player.h
 *
 *  Created on: Sep 7, 2016
 *      Author: zhangyalei
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Public_Struct.h"

class Player {
public:
	Player(void);
	virtual ~Player(void);

	inline void set_gate_cid(int gate_cid) { gate_cid_ = gate_cid; }
	inline int gate_cid(void) { return gate_cid_; }

	inline void set_game_cid(int game_cid) { game_cid_ = game_cid; }
	inline int game_cid(void) { return game_cid_; }

	inline void set_player_cid(int player_cid) { player_cid_ = player_cid; }
	inline int player_cid(void) { return player_cid_; }

	virtual void reset(void);
	virtual int link_close(void);

protected:
	Recycle_Tick recycle_tick_;

private:
	int gate_cid_;				//gate连接game的cid
	int game_cid_;				//game连接master的cid
	int player_cid_;			//player连接gate的cid
};

#endif /* PLAYER_H_ */
