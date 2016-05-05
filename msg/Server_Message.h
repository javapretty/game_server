/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

#ifndef __Game_Struct__H
#define __Game_Struct__H

#include "Block_Buffer.h"
#include "boost/unordered_map.hpp"
#include "Client_Message.h"

struct Account_Info : public MSG {
	std::string account;	
	int32_t agent_num;	
	int32_t server_num;	

	Account_Info(void);
	~Account_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Ip_Info : public MSG {
	std::string ip;	
	int32_t port;	

	Ip_Info(void);
	~Ip_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Player_DB_Cache : public MSG {
	int64_t role_id;	
	std::string account;	
	std::string role_name;	
	int32_t agent_num;	
	int32_t server_num;	
	int32_t level;	
	int32_t gender;	
	int32_t career;	

	Player_DB_Cache(void);
	~Player_DB_Cache();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Login_Player_Info : public MSG {
	std::string account;	
	std::string gate_ip;	
	int32_t gate_port;	
	std::string session;	
	int64_t session_tick;	

	Login_Player_Info(void);
	~Login_Player_Info();
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
	int32_t gender;	//0(女),1(男)
	int32_t career;	//职业1-3
	int32_t vip_level;	//vip等级

	Master_Player_Info(void);
	~Master_Player_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Game_Player_Info : public MSG {
	int64_t role_id;	//玩家id
	std::string account;	//玩家账号名
	std::string role_name;	//玩家名字
	std::string client_ip;	//客户端ip
	int32_t agent_num;	//平台编号
	int32_t server_num;	//服务器编号
	int32_t level;	//玩家等级
	int32_t exp;	//玩家经验
	int32_t gender;	//0(女),1(男)
	int32_t career;	//职业1-3
	int32_t create_time;	//创建角色时刻
	int32_t last_sign_in_time;	//最后登录时间
	int32_t last_sign_out_time;	//最后登出时间
	int32_t vitality;	//玩家体力
	int32_t buy_vitality_times;	//购买体力次数
	int32_t vip_level;	//vip等级
	int32_t vip_exp;	//vip经验值
	int32_t charge_gold;	//总共充值的元宝数
	int32_t skill_point;	//技能点
	int64_t recover_skill_time;	//回复技能点时间
	int32_t exchange_count;	//聚宝盆兑换剩余次数

	Game_Player_Info(void);
	~Game_Player_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Hero_Info : public MSG {
	boost::unordered_map<int32_t,Hero_Detail> hero_map;	

	Hero_Info(void);
	~Hero_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Bag_Info : public MSG {
	int32_t copper;	
	int32_t gold;	
	boost::unordered_map<int32_t,Item_Info> item_map;	

	Bag_Info(void);
	~Bag_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Mail_Info : public MSG {
	int32_t total_count;	//邮件的总数量，即目前为止收到的所有邮件数
	std::map<int32_t,Mail_Detail> mail_map;	

	Mail_Info(void);
	~Mail_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Shop_Info : public MSG {
	boost::unordered_map<int32_t,Shop_Detail> shop_detail;	

	Shop_Info(void);
	~Shop_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Player_Data : public MSG {
	int8_t status;	
	std::vector<int32_t> change_module;	
	Game_Player_Info player_info;	
	Hero_Info hero_info;	
	Bag_Info bag_info;	
	Mail_Info mail_info;	
	Shop_Info shop_info;	

	Player_Data(void);
	~Player_Data();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150000 : public MSG {

	MSG_150000(void);
	~MSG_150000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550000 : public MSG {
	std::vector<Player_DB_Cache> db_cache_vec;	

	MSG_550000(void);
	~MSG_550000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150001 : public MSG {
	Account_Info account_info;	

	MSG_150001(void);
	~MSG_150001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550001 : public MSG {
	Player_Data player_data;	

	MSG_550001(void);
	~MSG_550001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150002 : public MSG {
	Game_Player_Info player_info;	

	MSG_150002(void);
	~MSG_150002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550002 : public MSG {
	Player_Data player_data;	

	MSG_550002(void);
	~MSG_550002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150003 : public MSG {
	Player_Data player_data;	

	MSG_150003(void);
	~MSG_150003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550003 : public MSG {
	int64_t role_id;	

	MSG_550003(void);
	~MSG_550003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150004 : public MSG {
	int64_t role_id;	
	Mail_Detail mail_detail;	

	MSG_150004(void);
	~MSG_150004();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140000 : public MSG {
	std::string account;	
	std::string session;	
	std::string gate_ip;	
	int32_t gate_port;	

	MSG_140000(void);
	~MSG_140000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140001 : public MSG {
	std::string account;	

	MSG_140001(void);
	~MSG_140001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140100 : public MSG {

	MSG_140100(void);
	~MSG_140100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140200 : public MSG {
	Master_Player_Info player_info;	

	MSG_140200(void);
	~MSG_140200();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_180000 : public MSG {
	int32_t id;	
	std::string name;	

	MSG_180000(void);
	~MSG_180000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_180001 : public MSG {
	int64_t role_id;	
	std::string role_name;	
	std::string account;	
	uint16_t level;	
	std::string client_ip;	
	uint32_t login_time;	
	uint32_t logout_time;	
	uint32_t online_time;	

	MSG_180001(void);
	~MSG_180001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_160000 : public MSG {
	Master_Player_Info player_info;	

	MSG_160000(void);
	~MSG_160000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_160001 : public MSG {
	int64_t role_id;	

	MSG_160001(void);
	~MSG_160001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

#endif
