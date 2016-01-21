/**
 * struct Client_Game_Message description
 *
 * [This file was auto-generated. PLEASE DONT EDIT]
 *
 * @copyright urentech.com
 *
 */



#ifndef CLIENT_GAME_MESSAGE_H_
#define CLIENT_GAME_MESSAGE_H_

#include <cstring>
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

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	获取角色信息(返回)

	备注：

	状态：0：有角色，1：无角色
*/
struct MSG_520001 {

	Role_Info role_info;

	MSG_520001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

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

	MSG_120002(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	创建角色(返回)
*/
struct MSG_520002 {

	int64_t role_id;

	MSG_520002(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	获取背包信息

	备注：

	1、更新容量

	2、更新物品
*/
struct MSG_120100 {

	uint32_t bag_type;/*背包类型 背包：10000 仓库：20000*/

	MSG_120100(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	增加容量
*/
struct MSG_120101 {

	uint8_t pay_type;/*0 包裹，1 超级包裹 2 礼券 3元宝*/

	uint32_t bag_type;/*背包类型 背包：10000 仓库：20000*/

	uint16_t add_capacity;/*请求扩大的容量*/

	MSG_120101(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	丢弃物品
*/
struct MSG_120102 {

	std::vector<uint32_t> item_index_vec;/*指定格子中的物品被丢弃*/

	MSG_120102(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	移动物品
*/
struct MSG_120103 {

	uint32_t index_from;/**/

	uint32_t index_to;/**/

	MSG_120103(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	拆分物品
*/
struct MSG_120104 {

	uint32_t index;/*拆分的格子*/

	int16_t split_num;/*拆分出的数量*/

	MSG_120104(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	整理物品
*/
struct MSG_120105 {

	uint32_t bag_type;/*背包:10000 仓库:20000*/

	MSG_120105(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求合并物品

	备注：整理并合并物品，可能会将非绑定物品转成绑定物品以节约空间，同凡2的合并
*/
struct MSG_120106 {

	uint32_t bag_type;/*背包类型*/

	MSG_120106(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求出售物品

	备注：金钱的改变由主动消息通知，此消息不同于商店模块的，出售之后不能找回。
*/
struct MSG_120107 {

	std::vector<uint32_t> item_index_vec;/*指定格子中的物品被出售*/

	MSG_120107(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	获取背包信息(返回)
*/
struct MSG_520100 {

	uint32_t bag_type;/*背包类型*/

	int16_t capacity;/*背包容量*/

	std::vector<Item_Basic_Info> item_info_vec;

	MSG_520100(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	增加背包容量(返回)
*/
struct MSG_520101 {

	MSG_520101(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	丢弃物品(返回)
*/
struct MSG_520102 {

	MSG_520102(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	移动物品(返回)
*/
struct MSG_520103 {

	MSG_520103(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	拆分物品(返回)
*/
struct MSG_520104 {

	MSG_520104(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求合并物品(返回)
*/
struct MSG_520106 {

	MSG_520106(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求出售物品(返回)
*/
struct MSG_520107 {

	MSG_520107(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	获取邮件信息
*/
struct MSG_120200 {

	MSG_120200(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	收取附件
*/
struct MSG_120201 {

	int32_t mail_id; /*邮件ID,收取所有邮件附件时候id为0*/

	MSG_120201(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	删除邮件
*/
struct MSG_120202 {

	int32_t mail_id;/*邮件ID,删除所有邮件时候id为0*/

	MSG_120202(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	发送邮件
*/
struct MSG_120203 {

	Mail_Detail mail_detail;

	MSG_120203(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	获取邮件列表(返回)
*/
struct MSG_520200 {

	std::vector<Mail_Detail> mail_detail_vec;

	MSG_520200(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	收取附件(返回)
*/
struct MSG_520201 {

	std::vector<int32_t> mail_id_vec;/*成功收取附件的邮件id*/

	MSG_520201(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	删除邮件(返回)
*/
struct MSG_520202 {

	std::vector<int32_t> mail_id_vec;/*删除的邮件ID数组*/

	MSG_520202(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	发送邮件(返回)
*/
struct MSG_520203 {

	MSG_520203(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
