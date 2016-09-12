/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef PUBLIC_STURCT_H_
#define PUBLIC_STURCT_H_

#include "boost/unordered_map.hpp"
#include "Time_Value.h"
#include "Public_Define.h"
#include "Message.h"
#include "Inner_Msg.h"
#include "Base_Struct.h"

enum Log_Server_Type {
	LOG_MISC 				= 0,
	LOG_LOG_SERVER 		= 1,
	LOG_DB_SERVER			=	2,
	LOG_LOGIN_SERVER	=	3,
	LOG_MASTER_SERVER	=	4,
	LOG_GAME_SERVER		=	5,
	LOG_GATE_SERVER		=	6,
};

enum Error_Code {
	ERROR_SERVER_INNER 							= 10000,				//服务器内部错误
	ERROR_CLIENT_SESSION 						= 10001,				//session错误
	ERROR_CONNECT_TIMEOUT 						= 10002,				//连接超时
	ERROR_MSG_COUNT									= 10003,				//消息数量过多
	ERROR_MSG_SERIAL 								= 10004,				//消息序列号错误
	ERROR_MSG_TIME 									= 10005,				//消息时间错误
	ERROR_DISCONNECT_SELF 						= 10006,				//服务重启中,稍候再试
	ERROR_DISCONNECT_RELOGIN				= 10007,				//账号在其它地方登陆
	ERROR_LOGIN_FAIL									= 10008,				//登录失败
	ERROR_NOT_LOGIN									=	10009,				//玩家未登录
	ERROR_SESSION_TIMEOUT						=	10010,				//session到期
	ERROR_SESSION_SUCCESS						=	10011,				//login验证session成功，断开连接
};

enum Role_Status {
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

#define GET_CID(G_CID_,P_CID_) ((G_CID_) * 100000 + (P_CID_))
#define GET_GATE_CID(CID_) ((CID_) / 100000)
#define GET_GAME_CID(CID_) ((CID_) / 100000)
#define GET_PLAYER_CID(CID_) ((CID_) % 100000)

typedef boost::unordered_map<int, Base_Struct *> Struct_Id_Map;
typedef boost::unordered_map<std::string, Base_Struct *> Struct_Name_Map;
int load_struct(const char *path, Struct_Type struct_type, Struct_Id_Map &struct_id_map, Struct_Name_Map &struct_name_map);

struct Process_Info {
	int server_type;
	int server_id;
};

struct Server_Detail {
	int server_id;
	std::string server_ip;
	int server_port;
	int network_protocol;
};

typedef std::vector<Server_Detail> Server_List;
struct Server_Conf {
	Time_Value server_sleep_time;
	Time_Value receive_timeout;
	Time_Value server_send_interval;
	Time_Value connector_send_interval;

	Server_Detail log_server;
	Server_Detail db_server;
	Server_Detail login_client_server;
	Server_Detail login_gate_server;
	Server_Detail master_gate_server;
	Server_Detail master_game_server;
	Server_Detail master_http_server;
	Server_List game_list;
	Server_List gate_list;

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

struct Close_Info {
	int gate_cid;
	int player_cid;
	Time_Value timestamp;

	Close_Info(void) : gate_cid(-1), player_cid(-1), timestamp(Time_Value::zero) { }
	Close_Info(int gate_cid_, int player_cid_, const Time_Value &timestamp_): gate_cid(gate_cid_), player_cid(player_cid_), timestamp(timestamp_) { }
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

struct Player_DB_Cache {
	int64_t role_id;
	std::string role_name;
	std::string account;
	int agent_num;
	int server_num;

	Player_DB_Cache(void) { reset(); }
	void reset(void) {
		role_id = 0;
		role_name.clear();
		account.clear();
		agent_num = 0;
		server_num = 0;
	}
};

struct Session_Info {
	std::string session;
	Time_Value session_tick;
	std::string gate_ip;
	int gate_port;

	Session_Info(void) { reset(); }
	void reset(void) {
		session.clear();
		session_tick = Time_Value::zero;
		gate_ip.clear();
		gate_port = 0;
	}
};

struct Position {
	Position();
	Position(int posx, int posy, int posz);
	~Position();
	float operator-(Position &pos);
	void operator=(Position &pos);
	void set_position(int posx, int posy, int posz);
	void set_position(Position &pos);
	void reset();

	int x;
	int y;
	int z;
};

#endif /* PUBLIC_STURCT_H_ */
