/*
 * Login_Player.h
 *
 *  Created on: 2016年1月21日
 *      Author: root
 */

#ifndef LOGIN_SERVER_LOGIN_PLAYER_H_
#define LOGIN_SERVER_LOGIN_PLAYER_H_

#include "Public_Struct.h"

class Login_Player {
public:
	Login_Player(void);
	virtual ~Login_Player(void);

	void set_cid(int cid);
	int get_cid(void);
	void set_player_info(const Login_Player_Info &player_info);
	Login_Player_Info const &login_player_info(void) const;

	int respond_success_result(int msg_id, Block_Buffer *buf = 0);
	int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);

	void reset(void);

	int tick(Time_Value &now);
	void set_recycle(void);
	int recycle_status(void);
	int recycle_tick(const Time_Value &now);

	int register_timer(void);
	int unregister_timer(void);

	int link_close(void);

private:
	int cid_;
	bool is_register_timer_;
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
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

inline void Login_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);
}

inline int Login_Player::recycle_status(void) {
	return recycle_tick_.status_;
}

#endif /* LOGIN_SERVER_LOGIN_PLAYER_H_ */
