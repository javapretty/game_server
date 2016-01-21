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
#include "Public_Struct.h"


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

	uint32_t pack_type;/*背包类型 背包：20000 仓库：30000*/

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

	uint32_t pack_type;/*背包类型 背包：20000 仓库：30000 元神装备:320000 元神背包:330000*/

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

	std::vector<uint32_t> index;/*指定格子中的物品被丢弃*/

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

	uint32_t pack_type;/*背包:20000 仓库:30000*/

	MSG_120105(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	使用指定位置的物品

	备注：所有类型的物品都由此接口使用，由服务端判断使用物品的效果并以主动消息形式通知。
*/
struct MSG_120106 {

	uint32_t index;/*背包位置*/

	int32_t amount;/*使用数量*/

	int32_t e_value;/*拓展字段*/

	MSG_120106(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	使用指定型礼包
*/
struct MSG_120107 {

	int32_t item_index;/*道具index*/

	std::vector<int32_t> select_index_vector;/*选择的物品数组*/

	MSG_120107(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求合并物品

	备注：整理并合并物品，可能会将非绑定物品转成绑定物品以节约空间，同凡2的合并
*/
struct MSG_120108 {

	uint32_t pack_type;/*背包类型*/

	MSG_120108(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求出售物品

	备注：金钱的改变由主动消息通知，此消息不同于商店模块的，出售之后不能找回。
*/
struct MSG_120109 {

	std::vector<uint32_t> index;/*指定格子中的物品被出售*/

	MSG_120109(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	获取背包信息(返回)
*/
struct MSG_520100 {

	uint32_t pack_type;/*背包类型*/

	int16_t capacity;/*背包容量*/

	std::vector<Item_Basic_Info> item_info;

	MSG_520100(void);

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

	使用指定位置的物品(返回)
*/
struct MSG_520106 {

	uint32_t id;/*所使用物品的ID*/

	MSG_520106(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	使用指定型礼包(返回)
*/
struct MSG_520107 {

	MSG_520107(void);

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

	请求合并物品(返回)
*/
struct MSG_520108 {

	MSG_520108(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	请求出售物品(返回)
*/
struct MSG_520109 {

	MSG_520109(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
