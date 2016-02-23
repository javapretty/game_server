/**
 * struct Inner_Message description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef INNER_MESSAGE_H_
#define INNER_MESSAGE_H_

#include "Block_Buffer.h"
#include "Msg_Struct.h"


#include "Public_Struct.h"


/*

	配制文件热更消息
*/
struct MSG_400001 {

	std::string module;

	MSG_400001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
