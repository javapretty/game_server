/**
 * struct Gate_Message description
 *
 * [This file was auto-generated. PLEASE DONT EDIT]
 *
 * @copyright urentech.com
 *
 */



#ifndef GATE_MESSAGE_H_
#define GATE_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"
#include "Msg_Struct.h"
#include "Public_Struct.h"


/*

	gate-->login校验玩家accout session数据
*/
struct MSG_112000 {

	std::string account;

	std::string session;

	MSG_112000(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	login-->gate校验玩家结果
*/
struct MSG_112001 {

	std::string account;

	MSG_112001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
