/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

#ifndef __Msg_Struct__H
#define __Msg_Struct__H

#include "Block_Buffer.h"

struct MSG { 
	MSG(){}
	virtual ~MSG(){}
	virtual void serialize(Block_Buffer &buffer) const = 0; 
	virtual int deserialize(Block_Buffer &buffer) = 0; 
	virtual void reset(void) = 0; 
	virtual void print(void) = 0; 
};

struct Role_Info : public MSG {
	int64_t role_id;	//玩家id
	std::string account;	//玩家账号名
	std::string role_name;	//玩家名字
	int32_t level;	//玩家等级
	int32_t exp;	//玩家经验
	int32_t career;	//职业
	int32_t gender;	//0(女)1(男)
	int32_t vitality;	//玩家体力
	int32_t buy_vitality_times;	//今天购买次数
	int32_t vip_level;	//vip等级
	int32_t vip_exp;	//vip经验
	int32_t charge_gold;	//总共充值的元宝数
	int32_t skill_point;	//技能点

	Role_Info(void);
	~Role_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Property_Detail : public MSG {
	int32_t type;	//属性类型
	int32_t value;	//属性值

	Property_Detail(void);
	~Property_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Item_Info : public MSG {
	int32_t item_id;	
	int32_t amount;	
	int32_t level;	
	int32_t exp;	

	Item_Info(void);
	~Item_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Hero_Detail : public MSG {
	int32_t hero_id;	//
	int32_t level;	//英雄等级
	int32_t exp;	//英雄经验
	int32_t star;	//英雄星级
	int32_t quality;	//英雄品质等级
	int32_t energy;	//能量值，满的时候放大招
	std::vector<int32_t> skill_info;	//技能信息
	std::vector<Item_Info> equip_info;	//装备信息
	std::vector<Property_Detail> property_info;	//属性信息

	Hero_Detail(void);
	~Hero_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Mail_Detail : public MSG {
	bool pickup;	//是否收取过附件
	int32_t mail_id;	//邮件id
	int32_t send_time;	//发邮件时间
	int32_t sender_type;	//发件人类型
	int64_t sender_id;	//发件人id
	std::string sender_name;	//发件人名字
	std::string mail_title;	//邮件标题
	std::string mail_content;	//邮件内容
	int32_t copper;	//铜钱
	int32_t gold;	//元宝
	std::vector<Item_Info> item_info;	//道具信息

	Mail_Detail(void);
	~Mail_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Product_Info : public MSG {
	int32_t product_id;	
	int32_t item_id;	
	int32_t price;	
	int32_t count;	//剩余购买次数

	Product_Info(void);
	~Product_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Shop_Detail : public MSG {
	int32_t shop_type;	//商店类型，1是铜币，2是金币
	int32_t refresh_count;	
	std::vector<Product_Info> products;	

	Shop_Detail(void);
	~Shop_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300000 : public MSG {

	MSG_300000(void);
	~MSG_300000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300001 : public MSG {
	int32_t player_level;	
	int32_t player_exp;	

	MSG_300001(void);
	~MSG_300001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300002 : public MSG {
	int32_t vip_level;	
	int32_t vip_exp;	

	MSG_300002(void);
	~MSG_300002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300100 : public MSG {
	int32_t copper;	
	int32_t gold;	

	MSG_300100(void);
	~MSG_300100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300101 : public MSG {
	std::vector<Item_Info> item_info;	

	MSG_300101(void);
	~MSG_300101();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300200 : public MSG {
	std::vector<Mail_Detail> mail_info;	

	MSG_300200(void);
	~MSG_300200();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_300300 : public MSG {
	std::vector<Property_Detail> property_info;	

	MSG_300300(void);
	~MSG_300300();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120001 : public MSG {
	std::string account;	
	int64_t role_id;	
	std::string timestamp;	
	std::string ip;	
	int32_t agent_num;	
	int32_t server_num;	

	MSG_120001(void);
	~MSG_120001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520001 : public MSG {
	Role_Info role_info;	

	MSG_520001(void);
	~MSG_520001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120002 : public MSG {
	std::string account;	
	std::string role_name;	
	uint8_t gender;	//
	int32_t agent_num;	//平台编号
	int32_t server_num;	//服务器编号

	MSG_120002(void);
	~MSG_120002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520002 : public MSG {
	int64_t role_id;	

	MSG_520002(void);
	~MSG_520002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120003 : public MSG {

	MSG_120003(void);
	~MSG_120003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520003 : public MSG {
	int32_t vitality;	

	MSG_520003(void);
	~MSG_520003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120004 : public MSG {

	MSG_120004(void);
	~MSG_120004();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520004 : public MSG {
	int32_t copper;	

	MSG_520004(void);
	~MSG_520004();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120100 : public MSG {

	MSG_120100(void);
	~MSG_120100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520100 : public MSG {
	std::vector<Item_Info> item_info;	

	MSG_520100(void);
	~MSG_520100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120101 : public MSG {
	Item_Info item;	

	MSG_120101(void);
	~MSG_120101();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520101 : public MSG {

	MSG_520101(void);
	~MSG_520101();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120102 : public MSG {
	Item_Info item;	

	MSG_120102(void);
	~MSG_120102();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520102 : public MSG {

	MSG_520102(void);
	~MSG_520102();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120200 : public MSG {

	MSG_120200(void);
	~MSG_120200();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520200 : public MSG {
	std::vector<Mail_Detail> mail_info;	

	MSG_520200(void);
	~MSG_520200();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120201 : public MSG {
	int32_t mail_id;	//邮件ID,收取所有邮件附件时候id为0

	MSG_120201(void);
	~MSG_120201();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520201 : public MSG {
	std::vector<int32_t> mail_id_info;	//成功收取附件的邮件id

	MSG_520201(void);
	~MSG_520201();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120202 : public MSG {
	int32_t mail_id;	//邮件ID,删除所有邮件时候id为0

	MSG_120202(void);
	~MSG_120202();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520202 : public MSG {
	std::vector<int32_t> mail_id_info;	//删除的邮件ID数组

	MSG_520202(void);
	~MSG_520202();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120203 : public MSG {
	std::string receiver_name;	
	Mail_Detail mail_detail;	

	MSG_120203(void);
	~MSG_120203();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520203 : public MSG {

	MSG_520203(void);
	~MSG_520203();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120300 : public MSG {

	MSG_120300(void);
	~MSG_120300();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520300 : public MSG {
	std::vector<Hero_Detail> hero_info;	

	MSG_520300(void);
	~MSG_520300();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120301 : public MSG {
	int32_t hero_id;	

	MSG_120301(void);
	~MSG_120301();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520301 : public MSG {
	int32_t hero_id;	
	int32_t star;	//英雄星级

	MSG_520301(void);
	~MSG_520301();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120302 : public MSG {
	int32_t hero_id;	

	MSG_120302(void);
	~MSG_120302();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520302 : public MSG {
	int32_t hero_id;	
	int32_t quality;	//英雄品质等级

	MSG_520302(void);
	~MSG_520302();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120303 : public MSG {
	int32_t hero_id;	
	int32_t equip_index;	//
	std::vector<Item_Info> item_info;	//喂掉的装备列表

	MSG_120303(void);
	~MSG_120303();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520303 : public MSG {
	int32_t hero_id;	
	int32_t equip_index;	
	int32_t equip_level;	
	int32_t equip_exp;	

	MSG_520303(void);
	~MSG_520303();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120304 : public MSG {
	int32_t hero_id;	
	int32_t equip_index;	//
	bool on;	//
	Item_Info equip_info;	//装备信息

	MSG_120304(void);
	~MSG_120304();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520304 : public MSG {
	int32_t hero_id;	
	int32_t equip_index;	//
	bool on;	//
	Item_Info equip_info;	//装备信息

	MSG_520304(void);
	~MSG_520304();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120305 : public MSG {
	int32_t hero_id;	
	int32_t skill_id;	

	MSG_120305(void);
	~MSG_120305();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520305 : public MSG {
	int32_t hero_id;	
	int32_t skill_id;	

	MSG_520305(void);
	~MSG_520305();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120400 : public MSG {
	uint32_t shop_type;	//商店类型

	MSG_120400(void);
	~MSG_120400();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520400 : public MSG {
	Shop_Detail shop;	

	MSG_520400(void);
	~MSG_520400();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120401 : public MSG {
	uint32_t shop_type;	//商店类型
	uint32_t product_id;	
	uint32_t amount;	

	MSG_120401(void);
	~MSG_120401();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520401 : public MSG {

	MSG_520401(void);
	~MSG_520401();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_120402 : public MSG {
	uint32_t shop_type;	//商店类型

	MSG_120402(void);
	~MSG_120402();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_520402 : public MSG {

	MSG_520402(void);
	~MSG_520402();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100000 : public MSG {
	std::string account;	
	std::string password;	

	MSG_100000(void);
	~MSG_100000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500000 : public MSG {
	std::string ip;	//gate_ip
	int32_t port;	//gate_port
	std::string session;	

	MSG_500000(void);
	~MSG_500000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100001 : public MSG {
	std::string account;	
	std::string password;	

	MSG_100001(void);
	~MSG_100001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500001 : public MSG {
	std::string ip;	//gate_ip
	int32_t port;	//gate_port
	std::string session;	

	MSG_500001(void);
	~MSG_500001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100002 : public MSG {
	std::string account;	
	std::string session;	//登录验证的seesion

	MSG_100002(void);
	~MSG_100002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500002 : public MSG {
	std::string account;	

	MSG_500002(void);
	~MSG_500002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100003 : public MSG {
	int32_t client_time;	

	MSG_100003(void);
	~MSG_100003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500003 : public MSG {
	int32_t client_time;	
	int32_t server_time;	

	MSG_500003(void);
	~MSG_500003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_110001 : public MSG {
	int8_t chat_type;	//1世界聊天2私聊3帮派聊天4队伍聊天
	std::string chat_content;	
	std::string role_name;	//私聊时候发送对方名字

	MSG_110001(void);
	~MSG_110001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_510001 : public MSG {
	int8_t chat_type;	
	std::string chat_content;	
	std::string role_name;	
	int32_t gender;	
	int32_t career;	
	int32_t vip_level;	

	MSG_510001(void);
	~MSG_510001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

#endif
