/*
 * Login_Player.h
 *
 *  Created on: Jan 21, 2016
 *      Author: zhangyalei
 */

#ifndef LOGIN_PLAYER_H_
#define LOGIN_PLAYER_H_

#include "Player.h"

class Login_Player : public Player {
public:
	Login_Player(void);
	virtual ~Login_Player(void);

	void set_player_info(const Login_Player_Info &player_info) { player_info_ = player_info; }
	Login_Player_Info const &login_player_info(void) { return player_info_; }

	int tick(Time_Value &now);
	virtual void reset(void);
	virtual int link_close(void);

private:
	Login_Player_Info player_info_;
};

#endif /* LOGIN_PLAYER_H_ */
