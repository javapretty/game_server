/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef MASTER_PLAYER_H_
#define MASTER_PLAYER_H_

#include "Public_Struct.h"

class Master_Player {
public:
	Master_Player(void);
	virtual ~Master_Player(void);

	void set_cid_info(Cid_Info &cid_info);
	Cid_Info &cid_info(void);

	role_id_t role_id(void) { return player_info_.role_id; }
	Master_Player_Info const &master_player_info(void) const;

	int sign_in(Master_Player_Info &player_info);
	int sign_out(void);
	void reset(void);

	int tick(Time_Value &now);
	void set_recycle(void);
	int recycle_status(void);
	int recycle_tick(const Time_Value &now);

	int register_timer(void);
	int unregister_timer(void);

	int link_close(void);

private:
	bool is_register_timer_;
	Cid_Info cid_info_;
	Master_Player_Info player_info_;
	Recycle_Tick recycle_tick_;
};

////////////////////////////////////////////////////////////////////////////////
inline void Master_Player::set_cid_info(Cid_Info &cid_info) {
	cid_info_ = cid_info;
}

inline Cid_Info &Master_Player::cid_info(void) {
	return cid_info_;
}

inline Master_Player_Info const &Master_Player::master_player_info(void) const {
	return player_info_;
}

inline int Master_Player::link_close() {
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

inline void Master_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);
}

inline int Master_Player::recycle_status(void) {
	return recycle_tick_.status_;
}


#endif /* MASTER_PLAYER_H_ */
