/**
 * struct Game_DB_Message description
 *
 * [This file was auto-generated. PLEASE DONT EDIT]
 *
 * @copyright urentech.com
 *
 */



#ifndef GAME_DB_MESSAGE_H_
#define GAME_DB_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"
#include "Msg_Struct.h"
#include "Public_Struct.h"


/*

	加载 db_cache
*/
struct MSG_150000 {

	MSG_150000(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
struct MSG_550000 {

	std::vector<Player_DB_Cache> db_cache_vec;

	MSG_550000(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	加载玩家信息
*/
struct MSG_150001 {

	Account_Info account_info;

	MSG_150001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
struct MSG_550001 {

	Player_Data player_data;

	MSG_550001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	创建玩家
*/
struct MSG_150002 {

	Game_Player_Info player_info;

	MSG_150002(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
struct MSG_550002 {

	Player_Data player_data;

	MSG_550002(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	保存玩家信息
*/
struct MSG_150003 {

	Player_Data player_data;

	MSG_150003(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
struct MSG_550003 {

	int64_t role_id;

	MSG_550003(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
