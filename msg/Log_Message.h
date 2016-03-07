/**
 * struct Log_Message description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef LOG_MESSAGE_H_
#define LOG_MESSAGE_H_

#include "Block_Buffer.h"
#include "Msg_Struct.h"


#include "Public_Struct.h"



	//后台MySQL数据库插入测试
struct MSG_180000 {

	int32_t id;

	std::string name;

	MSG_180000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

	//后台登录退出流水
struct MSG_180001 {

	int64_t role_id;

	std::string role_name;

	std::string account;

	uint16_t level;

	std::string client_ip;

	uint32_t login_time;

	uint32_t logout_time;

	uint32_t online_time;

	MSG_180001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
