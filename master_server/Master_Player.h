/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef MASTER_PLAYER_H_
#define MASTER_PLAYER_H_

#include "Player.h"

class Master_Player : public Player {
public:
	Master_Player(void);
	virtual ~Master_Player(void);

	virtual int tick(Time_Value &now);
	virtual int link_close(bool server_close = false);
};

#endif /* MASTER_PLAYER_H_ */
