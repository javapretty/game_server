/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GATE_PLAYER_H_
#define GATE_PLAYER_H_

#include "Player.h"

class Gate_Player : public Player {
public:
	Gate_Player(void);
	virtual ~Gate_Player(void);

	virtual void reset(void);

	///验证包序列和时间戳 返回0为验证通过, -1操作过于频繁, -2包序号错, -3包时间戳错
	int verify_msg_info(uint32_t serial_cipher, uint32_t msg_time_cipher);

private:
	Msg_Info msg_info_;
};

#endif /* GATE_PLAYER_H_ */
