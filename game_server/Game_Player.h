/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_

#include "Public_Struct.h"
#include "Bag.h"
#include "Mail.h"

class Game_Player {
public:
	const static Time_Value game_player_save_interval_;

	Game_Player(void);
	virtual ~Game_Player(void);

	void set_cid_info(Cid_Info &cid_info);
	Cid_Info &cid_info(void);

	int respond_success_result(int msg_id, Block_Buffer *buf = 0);
	int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);

	Game_Player_Info const &game_player_info(void) const;
	Bag& bag(void) { return bag_; }
	Mail& mail(void) { return mail_; }

	int sign_in(std::string account);
	int sign_out(void);
	void reset(void);

	int sync_signin_to_master(void);
	int sync_signout_to_master(void);

	int tick(Time_Value &now);
	void set_recycle(void);
	int recycle_status(void);
	int recycle_tick(const Time_Value &now);

	int register_timer(void);
	int unregister_timer(void);

	int load_player_data(Player_Data &player_data);
	int save_player_data(Player_Data &player_data, bool is_logout);
	int save_player_data(bool is_logout = false);

	int link_close(void);

	int login_success(void);
	int respond_role_login(void);

private:
	bool is_register_timer_;
	Cid_Info cid_info_;		///登录信息，包括gate_cid和player_cid
	Game_Player_Info player_info_;
	Recycle_Tick recycle_tick_;
	Time_Value last_save_timestamp_;

	Bag bag_;
	Mail mail_;
};

////////////////////////////////////////////////////////////////////////////////
inline void Game_Player::set_cid_info(Cid_Info &cid_info) {
	cid_info_ = cid_info;
}

inline Cid_Info &Game_Player::cid_info(void) {
	return cid_info_;
}

inline Game_Player_Info const &Game_Player::game_player_info(void) const {
	return player_info_;
}

inline int Game_Player::link_close() {
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

inline void Game_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);
}

inline int Game_Player::recycle_status(void) {
	return recycle_tick_.status_;
}


#endif /* GAME_PLAYER_H_ */
