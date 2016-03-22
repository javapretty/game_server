/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef PUBLIC_STURCT_H_
#define PUBLIC_STURCT_H_

#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include "Time_Value.h"
#include "Misc.h"
#include "Game_Struct.h"

struct Cid_Info {
	int32_t gate_cid;			//gate连接game,master的cid
	int32_t game_cid;			//game连接master的cid
	int32_t player_cid;		//player连接gate的cid

	Cid_Info(void) : gate_cid(0), game_cid(0), player_cid(0) {};
	Cid_Info(int32_t gate_cid_, int32_t game_cid_, int32_t player_cid_) : gate_cid(gate_cid_), game_cid(game_cid_), player_cid(player_cid_) {};

	void reset(void) {
		gate_cid = -1;
		game_cid = -1;
		player_cid = -1;
	}

	bool operator==(const Cid_Info &cid_info) const {
		return gate_cid == cid_info.gate_cid && game_cid == cid_info.game_cid && player_cid == cid_info.player_cid;
	}
};

inline std::size_t hash_value(const Cid_Info &cid_info) {
    std::size_t seed = 0;
    boost::hash_combine(seed, cid_info.gate_cid);
    boost::hash_combine(seed, cid_info.game_cid);
    boost::hash_combine(seed, cid_info.player_cid);
    return seed;
}

struct Saving_Info {
	int64_t role_id;		// 角色
	Time_Value timestamp;	// 保存时的时间错

	Saving_Info(void) : role_id(0) {}
	Saving_Info(int64_t role_id_, Time_Value timestamp_) : role_id(role_id_), timestamp(timestamp_) {};
};

struct Close_Info {
	int cid;
	Time_Value timestamp;

	Close_Info(void) : cid(-1), timestamp(Time_Value::zero) { }
	Close_Info(int p_cid, const Time_Value &p_timestamp) : cid(p_cid), timestamp(p_timestamp) { }
};

struct Msg_Info {
	bool is_inited;
	int cid;
	long hash_key;								/// 用于加解密的hash key
	uint32_t msg_serial;					/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;	/// 操作频率统计
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

struct Tick_Info {
	const Time_Value server_info_interval_tick;
	Time_Value server_info_last_tick;

	const Time_Value player_interval_tick; /// Game_Player定时器间隔
	Time_Value player_last_tick;

	const Time_Value saving_scanner_interval_tick;	// 玩家下线保存表的扫描
	Time_Value saving_scanner_last_tick;

	const Time_Value object_pool_interval_tick;
	Time_Value object_pool_last_tick;

	Tick_Info(void)
	: server_info_interval_tick(Time_Value(300, 0)),
	  server_info_last_tick(Time_Value::zero),
	  player_interval_tick(Time_Value(0, 500 * 1000)),
	  player_last_tick(Time_Value::zero),
	  saving_scanner_interval_tick(20, 0),
	  saving_scanner_last_tick(Time_Value::zero),
	  object_pool_interval_tick(300, 0),
	  object_pool_last_tick(Time_Value::zero)
	{ }
};

struct Recycle_Tick {
	enum {
		NON_RECYCLE,	/// 非回收状态
		RECYCLE,		/// 回收状态
	};

	static const Time_Value recycle_time_;
	static const Time_Value tick_interval_;
	static const Time_Value session_interval_;

	int status_;
	Time_Value last_change_status_ts_;
	Time_Value last_tick_ts_;

	Recycle_Tick(void)
	: status_(NON_RECYCLE),
	  last_change_status_ts_(Time_Value::gettimeofday()),
	  last_tick_ts_(Time_Value::zero)
	{ }

	void reset(void) {
		set(NON_RECYCLE);
		last_tick_ts_ = Time_Value::zero;
	}

	void set(int v) {
		status_ = v;
		last_change_status_ts_ = Time_Value::gettimeofday();
	}
};

struct Player_Data {
	enum	{
		NULL_STATUS,
		SUCCESS_LOADED,			/// 加载成功
		SUCCESS_CREATED,		/// 创建成功
		ROLE_NOT_EXIST,			/// 角色不存在
		ROLE_HAS_EXIST,			///	角色已经存在
		ROLE_SAVE_OFFLINE,	/// 角色下线保存
	};

	enum {
		CHANGE_FIRST = 0,
		PLAYER_CHANGE,
		HERO_CHANGE,
		BAG_CHANGE,
		MAIL_CHANGE,
		CHANGE_END
	};

	int8_t status;
	boost::unordered_set<int> change_set;

	Player_Info player_info;
	Hero_Info hero_info;
	Bag_Info bag_info;
	Mail_Info mail_info;

	Player_Data(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	int load(int64_t role_id);
	int save(void);
	void reset(void);
	void set_all_change(bool is_change);
	void set_change(int change_id);
};

struct DB_Cache {
	const static int player_cache_map_bucket_num = 50000;

	DB_Cache(void)
	: id_player_cache_map(player_cache_map_bucket_num),
	  account_player_cache_map(player_cache_map_bucket_num)
	{ }

	boost::unordered_map<int64_t,Player_DB_Cache> id_player_cache_map;
	boost::unordered_map<std::string,Player_DB_Cache> account_player_cache_map;
};

#endif /* PUBLIC_STURCT_H_ */
