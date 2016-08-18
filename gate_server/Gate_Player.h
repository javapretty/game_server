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

	void set_player_cid(int cid) { player_cid_ = cid; }
	int get_player_cid(void) { return player_cid_; }
	void set_game_cid(int cid) { game_cid_ = cid; }
	int get_game_cid(void) { return game_cid_; }
	void set_account(std::string  &account) { account_ = account; }
	std::string& get_account(void) { return account_; }
	void reset(void);

	int tick(Time_Value &now);
	int link_close(void);

	/// 验证包序列和时间戳
	/// 返回0为验证通过, -1操作过于频繁, -2包序号错, -3包时间戳错
	int verify_msg_info(uint32_t serial_cipher, uint32_t msg_time_cipher);
	void reset_msg_info(void) { msg_info_.reset(); }

private:
	int player_cid_;		//client和gate连接的cid
	int game_cid_;				//player所在game_cid
	std::string account_;
	Msg_Info msg_info_;
	Recycle_Tick recycle_tick_;
};

#endif /* GATE_PLAYER_H_ */
