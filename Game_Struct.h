/** 
* struct Game_Struct description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Game_Struct__H
#define __Game_Struct__H

#include "Block_Buffer.h"
#include "Msg_Struct.h"

struct Account_Info {
	std::string account;	
	int32_t agent_num;	
	int32_t server_num;	

	Account_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Ip_Info {
	std::string ip;	
	int32_t port;	

	Ip_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Player_DB_Cache {
	int64_t role_id;	
	std::string account;	
	std::string role_name;	
	int32_t agent_num;	
	int32_t server_num;	
	int32_t level;	
	int32_t gender;	
	int32_t career;	

	Player_DB_Cache(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Login_Player_Info {
	std::string account;	
	std::string gate_ip;	
	int32_t gate_port;	
	std::string session;	
	int64_t session_tick;	

	Login_Player_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Master_Player_Info {
	int64_t role_id;	
	std::string account;	
	std::string role_name;	
	int32_t level;	
	int32_t gender;	//0(女),1(男)
	int32_t career;	//职业1-3
	int32_t vip_level;	//vip等级

	Master_Player_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Player_Info {
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

	Player_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Hero_Info {
	boost::unordered_map<int32_t,Hero_Detail> hero_map;	

	Hero_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Bag_Info {
	int32_t copper;	
	int32_t gold;	
	boost::unordered_map<int32_t,Item_Info> item_map;	

	Bag_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Mail_Info {
	int32_t total_count;	//邮件的总数量，即目前为止收到的所有邮件数
	std::map<int32_t,Mail_Detail> mail_map;	

	Mail_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Shop_Info {
	boost::unordered_map<int32_t,Shop_Detail> shop_detail;	

	Shop_Info(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
