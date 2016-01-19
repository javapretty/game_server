/*
 * MsgStruct.h
 *
 *  Created on: 2016年1月8日
 *      Author: zhangyalei
 */

#ifndef MSG_MSG_STRUCT_H_
#define MSG_MSG_STRUCT_H_

#include <cstring>
#include "Block_Buffer.h"

/*
角色信息
*/
struct Role_Info {
	int64_t role_id;
	std::string account;
	std::string role_name;
	uint8_t career;
	uint8_t gender;
	uint16_t level;

	Role_Info(void);
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

#endif /* MSG_MSG_STRUCT_H_ */
