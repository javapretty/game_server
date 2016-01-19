/*
 * GateMessage.h
 *
 *  Created on: 2015年12月28日
 *      Author: zhangyalei
 */

#ifndef MSG_GATE_MESSAGE_H_
#define MSG_GATE_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"

////////////此文件包含login,game,chat,master与gate通信的消息///////////////////


//gate同步玩家掉线到game
struct MSG_113000 {
	int64_t role_id;

	MSG_113000(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

//gate-->login校验玩家accout session数据
struct MSG_112000{
	std::string account;
	std::string session;

	MSG_112000(void);
	void serialize(Block_Buffer& w)const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

//login-->gate校验玩家结果
struct MSG_112001{
	std::string account;

	MSG_112001(void);
	void serialize(Block_Buffer& w)const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

#endif /* MSG_GATE_MESSAGE_H_ */
