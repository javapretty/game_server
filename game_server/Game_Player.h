/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_

#include "Player.h"

class Game_Player : public Player {
public:
	Game_Player(void);
	virtual ~Game_Player(void);

	virtual int tick(Time_Value &now);
	virtual int link_close(bool server_close = false);
};

#endif /* GAME_PLAYER_H_ */
