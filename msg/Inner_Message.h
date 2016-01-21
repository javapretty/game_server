/**
 * struct Inner_Message description
 *
 * [This file was auto-generated. PLEASE DONT EDIT]
 *
 * @copyright urentech.com
 *
 */



#ifndef INNER_MESSAGE_H_
#define INNER_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"
#include "Msg_Struct.h"
#include "Public_Struct.h"


/*

	配制文件热更消息
*/
struct MSG_400001 {

	std::string module;

	MSG_400001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
