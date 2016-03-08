/*
 * Game_Struct.h
 *
 *  Created on: 2016年3月8日
 *      Author: zhangyalei
 */

#ifndef GAME_STRUCT_H_
#define GAME_STRUCT_H_

#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include "Time_Value.h"
#include "Msg_Struct.h"

struct Account_Info {
	std::string account;
	int agent_num;
	int server_num;

	Account_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Ip_Info {
	std::string ip;
	int port;

	Ip_Info(void) { reset(); };

	void reset(void) {
		ip.clear();
		port = 0;
	}
};

struct Msg_Info {
	bool is_inited;
	int cid;
	long hash_key;				/// 用于加解密的hash key
	uint32_t msg_serial;				/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;		/// 操作频率统计
	Time_Value msg_interval_timestamp;

	void reset(void) {
		is_inited = false;
		cid = -1;
		hash_key = 0;
		msg_serial = 0;
		msg_timestamp = Time_Value::zero;
		msg_interval_count_ = 0;
		msg_interval_timestamp = Time_Value::zero;
	}
};

struct Login_Player_Info {
	Time_Value session_tick;
	std::string account;
	std::string gate_ip;
	int gate_port;
	std::string session;

	Login_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Gate_Player_Info {
	int64_t role_id;
	std::string account;

	Gate_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Master_Player_Info {
	int64_t role_id;
	std::string account;
	std::string role_name;
	int level;

	Master_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Game_Player_Info {
	int64_t role_id;					//玩家id
	std::string account;			//玩家账号名
	std::string role_name;		//玩家名字
	std::string client_ip;		//客户端ip
	int agent_num;						//平台编号
	int server_num;						//服务器编号
	int level;     			   		//玩家等级
	int exp;									//玩家经验
	int gender; 							//0(女),1(男)
	int career; 							//职业1-3
	int create_time;					//创建角色时刻
	int last_sign_in_time;		//最后登录时间
	int last_sign_out_time;		//最后登出时间
	int vitality;							//玩家体力
	int64_t last_change_time;	//上次更改体力时间
	int today_buy ;					//今天购买次数
	int vip_level;					//vip等级
	int vip_exp;						//vip经验值
	int charge_gold;				//总共充值的元宝数

	Game_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Hero_Info {
	Hero_Info();
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	boost::unordered_map<int32_t,Hero_Detail> hero_map;
};

struct Bag_Info {
	Bag_Info();
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	int32_t copper;
	int32_t gold;
	boost::unordered_map<int32_t,Item_Info> item_map;
};

struct Mail_Info {
	Mail_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	int total_count; 			//邮件的总数量，即目前为止收到的所有邮件数
	std::map<int,Mail_Detail> mail_map;
};

struct Player_DB_Cache {
	int64_t role_id;
	std::string account;
	std::string role_name;
	int agent_num;
	int server_num;
	int gender;
	int career;
	int level;

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif /* GAME_STRUCT_H_ */
