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

	inline void set_role_id(int64_t role_id) { role_id_ = role_id; }
	inline int64_t role_id(void) { return role_id_; }

	int tick(Time_Value &now);
	virtual void reset(void);
	virtual int link_close(void);

private:
	int64_t role_id_;			//player role_id
};

#endif /* MASTER_PLAYER_H_ */
