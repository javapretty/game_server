/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GATE_PLAYER_H_
#define GATE_PLAYER_H_

#include "Public_Struct.h"

class Gate_Player {
public:
	Gate_Player(void);
	virtual ~Gate_Player(void);

	void set_cid(int cid);
	int get_cid(void);
	void set_account(std::string  & account);
	void set_role_id(int64_t role_id);
	void reset(void);

	int tick(Time_Value &now);
	int link_close(void);
	void set_recycle(void);
	int recycle_status(void);
	int recycle_tick(const Time_Value &now);

	/// 验证包序列和时间戳
	/// 返回0为验证通过, -1操作过于频繁, -2包序号错, -3包时间戳错
	int verify_msg_info(uint32_t serial_cipher, uint32_t msg_time_cipher);
	void reset_msg_info(void) { msg_info_.reset(); }

private:
	int cid_;		//client和gate连接的cid
	Msg_Info msg_info_;
	Recycle_Tick recycle_tick_;
	Gate_Player_Info player_info_;
};

////////////////////////////////////////////////////////////////////////////////
inline void Gate_Player::set_cid(int cid) {
	cid_ = cid;
}

inline int Gate_Player::get_cid(void) {
	return cid_;
}

inline void Gate_Player::set_account(std::string &account) {
	player_info_.account = account;
}

inline void Gate_Player::set_role_id(int64_t role_id) {
	player_info_.role_id = role_id;
}

inline void Gate_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);
}

inline int Gate_Player::recycle_status(void) {
	return recycle_tick_.status_;
}


#endif /* GATE_PLAYER_H_ */
