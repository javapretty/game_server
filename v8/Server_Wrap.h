/*
 * Server_Wrap.h
 *
 *  Created on: 2016年3月30日
 *      Author: zhangyalei
 */

#ifndef SERVER_WRAP_H_
#define SERVER_WRAP_H_

#include "include/v8.h"

using namespace v8;

//////////////////////////////////GameServer相关函数////////////////////////////////
//函数说明：注册定时器到c++层		参数：1.定时器id 2.定时器间隔(毫秒单位) 3.从注册定时器到下次定时器到期中间间隔秒数	返回值：无
void register_game_timer(const FunctionCallbackInfo<Value>& args);
//函数说明：从c++层获取超时定时器	参数：无	返回值：到期的timer_id
void get_game_timer_id(const FunctionCallbackInfo<Value>& args);
//函数说明：新建game buffer	参数：无	返回值：buffer对象
void pop_game_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收game buffer	参数：1,buffer	返回值：无
void push_game_buffer(const FunctionCallbackInfo<Value>& args);

//函数说明：发送消息到gate_server 	参数：1.gate_cid 2.buffer对象	返回值：无
void send_game_buffer_to_gate(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息到master_server 	参数：1.buffer对象	返回值：无
void send_game_buffer_to_master(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息到db_server 	参数：1.buffer对象	返回值：无
void send_game_buffer_to_db(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息到log_server 	参数：1.buffer对象	返回值：无
void send_game_buffer_to_log(const FunctionCallbackInfo<Value>& args);

//函数说明：获取gate消息buffer	参数：无	返回值：buffer对象
void pop_game_gate_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收gate消息buffer	参数：1.gate_cid 2.buffer对象	返回值：无
void push_game_gate_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取db消息buffer	参数：无	返回值：buffer对象
void pop_game_db_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收db消息buffer	参数：1.db_cid 2.buffer对象	返回值：无
void push_game_db_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取master消息buffer	参数：无	返回值：buffer对象
void pop_game_master_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收master消息buffer	参数：1.master_cid 2.buffer对象	返回值：无
void push_game_master_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取下线玩家的cid		参数：无	返回值：cid值
void get_drop_game_player_cid(const FunctionCallbackInfo<Value>& args);

//函数说明：通过gate_cid获取game_player对象  	参数：1.gate_cid 2.player_cid	返回值：game_player对象
void get_game_player_by_gate_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：game_server关闭客户端连接  	参数：1.gate_cid 2.player_cid 3,error_code	返回值：无
void game_close_client(const FunctionCallbackInfo<Value>& args);

//////////////////////////////////MasterServer相关函数////////////////////////////////
//函数说明：注册定时器到c++层		参数：1.定时器id 2.定时器间隔(毫秒单位) 3.从注册定时器到下次定时器到期中间间隔秒数	返回值：无
void register_master_timer(const FunctionCallbackInfo<Value>& args);
//函数说明：从c++层获取超时定时器	参数：无	返回值：到期的timer_id
void get_master_timer_id(const FunctionCallbackInfo<Value>& args);
//函数说明：新建master buffer	参数：无	返回值：buffer对象
void pop_master_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收master buffer	参数：1,buffer	返回值：无
void push_master_buffer(const FunctionCallbackInfo<Value>& args);

//函数说明：发送数据到gate_server 	参数：1.gate_cid 2.buffer对象	返回值：无
void send_master_buffer_to_gate(const FunctionCallbackInfo<Value>& args);
//函数说明：发送数据到game_server 	参数：1.game_cid 2.buffer对象	返回值：无
void send_master_buffer_to_game(const FunctionCallbackInfo<Value>& args);
//函数说明：发送数据到db_server 	参数：1.buffer对象	返回值：无
void send_master_buffer_to_db(const FunctionCallbackInfo<Value>& args);
//函数说明：发送数据到log_server 	参数：1.buffer对象	返回值：无
void send_master_buffer_to_log(const FunctionCallbackInfo<Value>& args);

//函数说明：获取gate消息buffer	参数：无	返回值：buffer对象
void pop_master_gate_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收gate消息buffer	参数：1.gate_cid 2.buffer对象	返回值：无
void push_master_gate_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取db消息buffer	参数：无	返回值：buffer对象
void pop_master_db_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收db消息buffer	参数：1.db_cid 2.buffer对象	返回值：无
void push_master_db_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取master消息buffer	参数：无	返回值：buffer对象
void pop_master_game_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收master消息buffer	参数：1.game_cid 2.buffer对象	返回值：无
void push_master_game_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取下线玩家的cid		参数：无	返回值：cid值
void get_drop_master_player_cid(const FunctionCallbackInfo<Value>& args);

//函数说明：通过gate_cid获取master_player对象  	参数：1,gate_cid 2,player_cid	返回值：master_player对象
void get_master_player_by_gate_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：通过game_cid获取master_player对象  	参数：1,game_cid 2,player_cid	返回值：master_player对象
void get_master_player_by_game_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：master_server关闭客户端连接  	参数：1,gate_cid 2,player_cid 3,error_code	返回值：无
void master_close_client(const FunctionCallbackInfo<Value>& args);

#endif /* SERVER_WRAP_H_ */
