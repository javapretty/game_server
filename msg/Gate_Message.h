/**
 * struct Gate_Message description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef GATE_MESSAGE_H_
#define GATE_MESSAGE_H_

#include "Block_Buffer.h"
#include "Msg_Struct.h"


#include "Public_Struct.h"


/*

	gate-->login校验玩家accout session数据
*/
struct MSG_112000 {

	std::string account;

	std::string session;

	std::string gate_ip;

	int32_t gate_port;

	MSG_112000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	login-->gate校验玩家结果
*/
struct MSG_112001 {

	std::string account;

	MSG_112001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
