/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef PUBLIC_STURCT_H_
#define PUBLIC_STURCT_H_

#include "Time_Value.h"
#include "Misc.h"
#include "Message.h"
#include "Inner_Msg.h"

enum Error_Code {
	ERROR_SERVER_INNER 							= 10000,				//服务器内部错误
	ERROR_CLIENT_SESSION 						= 10001,				//session错误
	ERROR_CONNECT_TIMEOUT 					= 10002,				//连接超时
	ERROR_MSG_COUNT									= 10003,				//消息数量过多
	ERROR_MSG_SERIAL 								= 10004,				//消息序列号错误
	ERROR_MSG_TIME 									= 10005,				//消息时间错误
	ERROR_REGISTER_VERIFY_FAIL				= 10006,				//注册验证失败
	ERROR_LOGIN_VERIFY_FAIL					= 10007,				//登录验证失败
	ERROR_DISCONNECT_SELF 					= 10008,				//服务重启中,稍候再试
	ERROR_DISCONNECT_RELOGIN				= 10009,				//账号在其它地方登陆
};

enum	 Role_Status {
	ROLE_SUCCESS_LOAD 	= 0,		//加载成功
	ROLE_SUCCESS_CREATE = 0,	//创建成功
	ROLE_NOT_EXIST 			= 1,		//角色不存在
	ROLE_HAS_EXIST 			= 2,		//	角色已经存在
	ROLE_SAVE_OFFLINE 	= 3,		//角色下线保存
};

enum Server_Status {
	STATUS_NORMAL = 1,				//正常状态
	STATUS_CLOSING = 2,				//关闭中状态
};

enum Server_Type {
	MULTI_PROCESS = 1,				//多进程
	MULTI_THREAD = 2,					//多线程
};

enum Struct_Type {
	MONGO_STRUCT = 1,					//mongo数据库结构体
	MYSQL_STRUCT = 2,					//mysql数据库结构体
	LOG_STRUCT = 3,						//日志数据库结构体
	MSG_STRUCT = 4,						//消息结构体
};

class Base_Struct;
typedef boost::unordered_map<int32_t, Base_Struct *> Struct_Id_Map;
typedef boost::unordered_map<std::string, Base_Struct *> Struct_Name_Map;

int load_struct(const char *path, Struct_Type struct_type, Struct_Id_Map &struct_id_map, Struct_Name_Map &struct_name_map);

struct Field_Info {
	std::string field_label;
	std::string field_type;
	std::string field_name;
	std::string key_type;
	std::string key_name;
};

struct Server_Conf {
	Time_Value server_sleep_time;
	Time_Value receive_timeout;
	Time_Value server_send_interval;
	Time_Value connector_send_interval;

	std::string server_ip;
	int log_port;
	int db_port;
	int login_client_network_protocol;
	int login_client_port;
	int login_gate_port;
	int master_gate_port;
	int master_game_port;
	int game_gate_port;
	int gate_client_network_protocol;
	int gate_client_port;

	void init_server_conf(void);
};

struct V8_Timer_Handler {
	int timer_id; 				//js层定时器编号
	int interval; 				//定时时间间隔(毫秒为单位)
	Time_Value next_tick;	//下一次执行时间
};

class V8_Timer_Compare {
public:
	inline bool operator()(V8_Timer_Handler *t1, V8_Timer_Handler *t2) {
		return t1->next_tick > t2->next_tick;
	}
};

struct Ip_Info {
	std::string ip;
	int32_t port;

	Ip_Info(void): ip("127.0.0.1"), port(0) {}
	Ip_Info(std::string &ip_, int port_): ip(ip_), port(port_) {}
};

struct Close_Info {
	int cid;
	Time_Value timestamp;

	Close_Info(void) : cid(-1), timestamp(Time_Value::zero) { }
	Close_Info(int cid_, const Time_Value &timestamp_): cid(cid_), timestamp(timestamp_) { }
};

struct Msg_Info {
	bool is_inited;
	int cid;
	int hash_key;								/// 用于加解密的hash key
	uint32_t msg_serial;					/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;	/// 操作频率统计
	Time_Value msg_interval_timestamp;

	Msg_Info() { reset(); }
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

	const Time_Value object_pool_interval_tick;
	Time_Value object_pool_last_tick;

	Tick_Info(void)
	: server_info_interval_tick(Time_Value(300, 0)),
	  server_info_last_tick(Time_Value::zero),
	  player_interval_tick(Time_Value(0, 500 * 1000)),
	  player_last_tick(Time_Value::zero),
	  object_pool_interval_tick(300, 0),
	  object_pool_last_tick(Time_Value::zero)
	{ }
};

struct Recycle_Tick {
	enum {
		NON_RECYCLE,	/// 非回收状态
		RECYCLE,		/// 回收状态
	};

	int status;
	Time_Value recycle_tick;

	Recycle_Tick(void): status(NON_RECYCLE) {}

	void reset(void) {
		set(NON_RECYCLE);
	}

	void set(int s) {
		status = s;
		recycle_tick = Time_Value::gettimeofday() + Time_Value(2, 0);
	}
};

struct Player_DB_Cache {
	int64_t role_id;
	std::string role_name;
	std::string account;
	int32_t agent_num;
	int32_t server_num;
	int32_t level;

	Player_DB_Cache(void) { reset(); }
	void reset(void) {
		role_id = 0;
		role_name.clear();
		account.clear();
		agent_num = 0;
		server_num = 0;
		level = 0;
	}
};

struct Login_Player_Info {
	std::string account;
	std::string gate_ip;
	int32_t gate_port;
	std::string session;
	int64_t session_tick;

	Login_Player_Info(void) { reset(); }
	void reset(void) {
		account.clear();
		gate_ip.clear();
		gate_port = 0;
		session.clear();
		session_tick = 0;
	}
};

#endif /* PUBLIC_STURCT_H_ */
