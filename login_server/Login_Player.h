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

	inline void set_session_info(const Session_Info &session_info) { session_info_ = session_info; }
	inline const Session_Info &session_info(void) { return session_info_; }

	virtual void reset(void);
	virtual int tick(Time_Value &now);

private:
	Session_Info session_info_;
};

#endif /* LOGIN_PLAYER_H_ */
