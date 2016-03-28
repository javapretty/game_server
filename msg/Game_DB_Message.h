/** 
* struct Game_DB_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Game_DB_Message__H
#define __Game_DB_Message__H

#include "Block_Buffer.h"
#include "Public_Struct.h"

struct MSG_150000 {

	MSG_150000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_550000 {
	std::vector<Player_DB_Cache> db_cache_vec;	

	MSG_550000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_150001 {
	Account_Info account_info;	

	MSG_150001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_550001 {
	Player_Data player_data;	

	MSG_550001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_150002 {
	Game_Player_Info player_info;	

	MSG_150002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_550002 {
	Player_Data player_data;	

	MSG_550002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_150003 {
	Player_Data player_data;	

	MSG_150003(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_550003 {
	int64_t role_id;	

	MSG_550003(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_150004 {
	int64_t role_id;	
	Mail_Detail mail_detail;	

	MSG_150004(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
