/*
 * Login_Player.h
 *
 *  Created on: 2016年1月21日
 *      Author: zhangyalei
 */

#ifndef LOGIN_PLAYER_H_
#define LOGIN_PLAYER_H_

#include "Public_Struct.h"

class Login_Player {
public:
	Login_Player(void);
	virtual ~Login_Player(void);

	void set_cid(int cid);
	int get_cid(void);
	void set_player_info(const Login_Player_Info &player_info);
	Login_Player_Info const &login_player_info(void) const;

	void reset(void);
	int tick(Time_Value &now);
	int link_close(void);

private:
	int cid_;
	Recycle_Tick recycle_tick_;
	Login_Player_Info player_info_;
};

////////////////////////////////////////////////////////////////////////////////
inline void Login_Player::set_cid(int cid) {
	cid_ = cid;
}

inline int Login_Player::get_cid(void) {
	return cid_;
}

inline void Login_Player::set_player_info(const Login_Player_Info &player_info) {
	player_info_ = player_info;
}

inline Login_Player_Info const &Login_Player::login_player_info(void) const {
	return player_info_;
}

inline int Login_Player::link_close() {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE)
		return 0;

	recycle_tick_.set(Recycle_Tick::RECYCLE);
	//修改session_tick的值，防止login_manager对同一个链接重复push_drop
	player_info_.session_tick += 10;
	return 0;
}

#endif /* LOGIN_PLAYER_H_ */
