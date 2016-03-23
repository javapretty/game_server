/** 
* struct Game_Master_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Game_Master_Message__H
#define __Game_Master_Message__H

#include "Block_Buffer.h"
#include "Public_Struct.h"

struct MSG_160001 {
	Master_Player_Info player_info;	

	MSG_160001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_160002 {
	int64_t role_id;	

	MSG_160002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
