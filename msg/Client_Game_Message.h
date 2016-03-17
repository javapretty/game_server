/** 
* struct Client_Game_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.0]
*
*/

#ifndef __Client_Game_Message__H
#define __Client_Game_Message__H

#include "Block_Buffer.h"
#include "Msg_Struct.h"

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

struct MSG_120002 {
	std::string account;	
	std::string role_name;	
	uint8_t gender;	//
	int32_t agent_num;	//平台编号
	int32_t server_num;	//服务器编号

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

struct MSG_120003 {

	MSG_120003(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520003 {
	int32_t vitality;	

	MSG_520003(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120100 {

	MSG_120100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520100 {
	std::vector<Item_Info> item_info;	

	MSG_520100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120101 {
	Item_Info item;	

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

struct MSG_120102 {
	Item_Info item;	

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

struct MSG_120200 {

	MSG_120200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520200 {
	std::vector<Mail_Detail> mail_info;	

	MSG_520200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120201 {
	int32_t mail_id;	//邮件ID,收取所有邮件附件时候id为0

	MSG_120201(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520201 {
	std::vector<int32_t> mail_id_info;	//成功收取附件的邮件id

	MSG_520201(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120202 {
	int32_t mail_id;	//邮件ID,删除所有邮件时候id为0

	MSG_120202(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520202 {
	std::vector<int32_t> mail_id_info;	//删除的邮件ID数组

	MSG_520202(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

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

struct MSG_120300 {

	MSG_120300(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520300 {
	std::vector<Hero_Detail> hero_info;	

	MSG_520300(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120301 {
	int32_t hero_id;	

	MSG_120301(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520301 {
	int32_t hero_id;	
	int32_t star;	//英雄星级

	MSG_520301(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120302 {
	int32_t hero_id;	

	MSG_120302(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520302 {
	int32_t hero_id;	
	int32_t quality;	//英雄品质等级

	MSG_520302(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120303 {
	int32_t hero_id;	
	int32_t equip_index;	//
	std::vector<Item_Info> item_info;	//喂掉的装备列表

	MSG_120303(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520303 {
	int32_t hero_id;	
	int32_t equip_index;	
	int32_t equip_level;	
	int32_t equip_exp;	

	MSG_520303(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120304 {
	int32_t hero_id;	
	int32_t equip_index;	//
	bool on;	//
	Item_Info equip_info;	//装备信息

	MSG_120304(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520304 {
	int32_t hero_id;	
	int32_t equip_index;	//
	bool on;	//
	Item_Info equip_info;	//装备信息

	MSG_520304(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_120305 {
	int32_t hero_id;	
	int32_t skill_id;	

	MSG_120305(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_520305 {
	int32_t hero_id;	
	int32_t skill_id;	

	MSG_520305(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
