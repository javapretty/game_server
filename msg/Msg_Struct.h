/**
 * struct Msg_Struct description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef MSG_STRUCT_H_
#define MSG_STRUCT_H_

#include "Block_Buffer.h"
struct Role_Info {

	int64_t role_id;

	std::string account;

	std::string role_name;

	int32_t level;

	int32_t career;

	int32_t gender;

	Role_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct Item_Basic_Info {

	int32_t id; //物品id

	int32_t amount; //物品数量

	Item_Basic_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct Mail_Detail {

	bool pickup; //是否收取过附件

	int32_t mail_id; //邮件id

	int32_t send_time; //发邮件时间

	int32_t sender_type; //发件人类型

	int64_t sender_id; //发件人id

	std::string sender_name; //发件人名字

	std::string mail_title; //邮件标题

	std::string mail_content; //邮件内容

	int32_t copper; //铜钱

	int32_t gold; //元宝

	Mail_Detail(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
