/**
 * struct Client_Game_Message description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef CLIENT_GAME_MESSAGE_H_
#define CLIENT_GAME_MESSAGE_H_

#include "Block_Buffer.h"
#include "Msg_Struct.h"


/*

	获取角色信息
*/
struct MSG_120001 {

	std::string account;

	int64_t role_id;

	std::string timestamp;

	std::string ip;

	int32_t agent_num;

	int32_t server_num;

	MSG_120001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520001 {

	Role_Info role_info;

	MSG_520001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	创建角色
*/
struct MSG_120002 {

	std::string account;

	std::string role_name;

	uint8_t gender; /*0 女，1 男*/

	int32_t agent_num; /*平台*/

	int32_t server_num; /*服*/

	MSG_120002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520002 {

	int64_t role_id;

	MSG_520002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	获取背包信息
*/
struct MSG_120100 {

	MSG_120100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520100 {

	std::vector<Item_Basic_Info> item_info_vec;

	MSG_520100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	使用物品
*/
struct MSG_120101 {

	int32_t item_id; //物品id

	int32_t item_amount; //物品数量

	MSG_120101(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520101 {

	MSG_520101(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	出售物品
*/
struct MSG_120102 {

	int32_t item_id; //物品id

	int32_t item_amount; //物品数量

	MSG_120102(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520102 {

	MSG_520102(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	获取邮件信息
*/
struct MSG_120200 {

	MSG_120200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520200 {

	std::vector<Mail_Detail> mail_detail_vec;

	MSG_520200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	收取附件
*/
struct MSG_120201 {

	int32_t mail_id; /*邮件ID,收取所有邮件附件时候id为0*/

	MSG_120201(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520201 {

	std::vector<int32_t> mail_id_vec;/*成功收取附件的邮件id*/

	MSG_520201(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	删除邮件
*/
struct MSG_120202 {

	int32_t mail_id;/*邮件ID,删除所有邮件时候id为0*/

	MSG_120202(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520202 {

	std::vector<int32_t> mail_id_vec;/*删除的邮件ID数组*/

	MSG_520202(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	发送邮件
*/
struct MSG_120203 {

	std::string receiver_name;

	Mail_Detail mail_detail;

	MSG_120203(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520203 {

	MSG_520203(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	获取英雄信息
*/
struct MSG_120300 {

	MSG_120300(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520300 {

	std::vector<Hero_Detail> hero_detail_vec;

	MSG_520300(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	提高英雄星级
*/
struct MSG_120301 {

	int32_t hero_id;

	MSG_120301(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520301 {

	int32_t hero_id;

	int32_t star; //英雄星级

	MSG_520301(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	提高英雄品质
*/
struct MSG_120302 {

	int32_t hero_id;

	MSG_120302(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_520302 {

	int32_t hero_id;

	int32_t quality; //英雄品质等级

	MSG_520302(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
