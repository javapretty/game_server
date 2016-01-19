/*
 * Game_Master_Message.h
 *
 *  Created on: 2016年1月11日
 *      Author: zhangyalei
 */

#ifndef MSG_GAME_MASTER_MESSAGE_H_
#define MSG_GAME_MASTER_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"
#include "Public_Struct.h"


struct MSG_160001 {
	Master_Player_Info player_info;

	MSG_160001(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

struct MSG_160002 {
	int64_t role_id;

	MSG_160002(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

#endif /* MSG_GAME_MASTER_MESSAGE_H_ */
