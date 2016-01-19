/*
 * ClientToGame.h
 *
 *  Created on: 2015年12月23日
 *      Author: zhangyalei
 */

#ifndef MSG_CLIENTTOGAME_H_
#define MSG_CLIENTTOGAME_H_

#include <Msg_Struct.h>
#include <cstring>
#include "Block_Buffer.h"

/*
获取角色信息
*/
struct MSG_120001 {
	std::string account;/*账号*/
	int64_t role_id;/**/
	std::string timestamp;/**/
	std::string ip;/**/
	int32_t agent_num;/*平台*/
	int32_t server_num;/*服*/

	MSG_120001(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

/*
获取角色信息(返回)
备注：
状态：0：有角色，1：无角色
*/
struct MSG_520001 {
	Role_Info role_info;

	MSG_520001(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

/*
创建角色
*/
struct MSG_120002 {
	std::string account;/**/
	std::string role_name;/**/
	uint8_t gender;/*0 女，1 男*/
	int32_t agent_num;/*平台*/
	int32_t server_num;/*服*/

	MSG_120002(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

/*
创建角色(返回)
*/
struct MSG_520002 {
	int64_t role_id;

	MSG_520002(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

#endif /* MSG_CLIENTTOGAME_H_ */
