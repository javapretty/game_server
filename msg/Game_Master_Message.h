/**
 * struct Game_Master_Message description
 *
 * [This file was auto-generated. PLEASE DONT EDIT]
 *
 * @copyright urentech.com
 *
 */



#ifndef GAME_MASTER_MESSAGE_H_
#define GAME_MASTER_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"
#include "Msg_Struct.h"
#include "Public_Struct.h"


struct MSG_160001 {

	Master_Player_Info player_info;

	MSG_160001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
struct MSG_160002 {

	int64_t role_id;

	MSG_160002(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
