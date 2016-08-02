/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.4]
*/

#ifndef __game_db_struct__H
#define __game_db_struct__H

#include "Block_Buffer.h"
#include "boost/unordered_map.hpp"

struct MSG { 
	MSG(){}
	virtual ~MSG(){}
	virtual void serialize(Block_Buffer &buffer) const = 0; 
	virtual int deserialize(Block_Buffer &buffer) = 0; 
	virtual void reset(void) = 0; 
	virtual void print(void) = 0; 
};

struct Cid_Info;
struct Create_Role_Info;
struct Create_Guild_Info;
struct Role_Info;
struct Hero_Detail;
struct Item_Info;
struct Property_Detail;
struct Mail_Detail;
struct Shop_Detail;
struct Product_Info;
struct Guild_Member_Detail;
struct Rank_Member;
struct Master_Player_Info;
struct Game_Player_Info;
struct Hero_Info;
struct Bag_Info;
struct Mail_Info;
struct Shop_Info;
struct Guild_Info;
struct Offline_Info;
struct Rank_Info;

struct Cid_Info : public MSG {
	int32_t gate_cid;	
	int32_t player_cid;	

	Cid_Info(void);
	~Cid_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Create_Role_Info : public MSG {
	std::string account;	
	std::string role_name;	
	std::string client_ip;	
	int8_t gender;	
	int8_t career;	

	Create_Role_Info(void);
	~Create_Role_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Create_Guild_Info : public MSG {
	int64_t guild_id;	
	std::string guild_name;	
	int64_t chief_id;	

	Create_Guild_Info(void);
	~Create_Guild_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Role_Info : public MSG {
	int64_t role_id;	//玩家id
	std::string role_name;	//玩家名字
	std::string account;	//玩家账号名
	int32_t level;	//玩家等级
	int32_t exp;	//玩家经验
	int8_t gender;	//0(女)1(男)
	int8_t career;	//职业
	int32_t vitality;	//玩家体力
	int32_t buy_vitality_times;	//今天购买次数
	int32_t vip_level;	//vip等级
	int32_t vip_exp;	//vip经验
	int32_t charge_gold;	//总共充值的元宝数

	Role_Info(void);
	~Role_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Hero_Detail : public MSG {
	int32_t hero_id;	
	int32_t level;	
	int32_t exp;	
	int32_t star;	
	int32_t quality;	
	int32_t energy;	
	std::vector<int32_t> skill_info;	
	std::vector<Item_Info> equip_info;	
	std::vector<Property_Detail> property_info;	

	Hero_Detail(void);
	~Hero_Detail();
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

struct Property_Detail : public MSG {
	int32_t type;	
	int32_t name;	

	Property_Detail(void);
	~Property_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Mail_Detail : public MSG {
	bool pickup;	
	int32_t mail_id;	
	int32_t send_time;	
	int32_t sender_type;	
	int64_t sender_id;	
	std::string sender_name;	
	std::string mail_title;	
	std::string mail_content;	
	int32_t copper;	
	int32_t gold;	
	std::vector<Item_Info> item_info;	

	Mail_Detail(void);
	~Mail_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Shop_Detail : public MSG {
	int32_t shop_type;	
	int32_t refresh_count;	
	std::vector<Product_Info> product_info;	

	Shop_Detail(void);
	~Shop_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Product_Info : public MSG {
	int32_t product_id;	
	int32_t item_id;	
	int32_t price;	
	int32_t count;	

	Product_Info(void);
	~Product_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Guild_Member_Detail : public MSG {
	int64_t role_id;	
	std::string role_name;	
	int32_t level;	
	int32_t career;	

	Guild_Member_Detail(void);
	~Guild_Member_Detail();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Rank_Member : public MSG {
	int64_t role_id;	
	std::string role_name;	
	int32_t value;	

	Rank_Member(void);
	~Rank_Member();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Master_Player_Info : public MSG {
	int64_t role_id;	
	std::string account;	
	std::string role_name;	
	int32_t level;	
	int32_t gender;	
	int32_t career;	
	int32_t vip_level;	

	Master_Player_Info(void);
	~Master_Player_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Game_Player_Info : public MSG {
	int64_t role_id;	
	std::string role_name;	
	std::string account;	
	std::string client_ip;	
	int32_t agent_num;	
	int32_t server_num;	
	int32_t level;	
	int32_t exp;	
	int8_t gender;	
	int8_t career;	
	int32_t create_time;	
	int32_t login_time;	
	int32_t logout_time;	
	int32_t vitality;	
	int32_t buy_vitality_times;	
	int32_t vip_level;	
	int32_t vip_exp;	
	int32_t charge_gold;	
	int32_t guild_id;	
	std::string guild_name;	

	Game_Player_Info(void);
	~Game_Player_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Hero_Info : public MSG {
	int64_t role_id;	
	boost::unordered_map<int32_t, Hero_Detail> hero_map;	

	Hero_Info(void);
	~Hero_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Bag_Info : public MSG {
	int64_t role_id;	
	int32_t copper;	
	int32_t gold;	
	boost::unordered_map<int32_t, Item_Info> item_map;	

	Bag_Info(void);
	~Bag_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Mail_Info : public MSG {
	int64_t role_id;	
	int32_t total_count;	
	boost::unordered_map<int32_t, Mail_Detail> mail_map;	

	Mail_Info(void);
	~Mail_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Shop_Info : public MSG {
	int64_t role_id;	
	boost::unordered_map<int32_t, Shop_Detail> shop_map;	

	Shop_Info(void);
	~Shop_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Guild_Info : public MSG {
	int64_t guild_id;	
	std::string guild_name;	
	int64_t chief_id;	
	int32_t create_time;	
	bool is_change;	
	std::vector<Guild_Member_Detail> apply_list;	
	std::vector<Guild_Member_Detail> member_list;	

	Guild_Info(void);
	~Guild_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Offline_Info : public MSG {
	int64_t role_id;	
	int64_t guild_id;	
	std::string guild_name;	
	int32_t flag;	

	Offline_Info(void);
	~Offline_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Rank_Info : public MSG {
	int64_t rank_type;	
	int32_t min_value;	
	int64_t min_role_id;	
	boost::unordered_map<int64_t, Rank_Member> member_map;	

	Rank_Info(void);
	~Rank_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

#endif
