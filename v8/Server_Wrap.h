/*
 * Server_Wrap.h
 *
 *  Created on: Mar 30, 2016
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

//函数说明：获取gate消息object	参数：无	返回值：消息object
void pop_game_gate_msg_object(const FunctionCallbackInfo<Value>& args);
//函数说明：获取master消息object	参数：无	返回值：消息object
void pop_game_master_msg_object(const FunctionCallbackInfo<Value>& args);
//函数说明：获取db消息object	参数：无	返回值：消息object
void pop_game_db_msg_object(const FunctionCallbackInfo<Value>& args);

//函数说明：发送消息object到gate_server	参数：1.gate_cid 2.player_cid 3.msg_id 4.status 5.消息object	返回值：无
void send_game_msg_to_gate(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到master_server	参数：1.player_cid 2.msg_id 3.status 4.消息object	返回值：无
void send_game_msg_to_master(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到db_server	参数：1.msg_id 2.消息object	返回值：无
void send_game_msg_to_db(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到log_server	参数：1.msg_id 2.消息object	返回值：无
void send_game_msg_to_log(const FunctionCallbackInfo<Value>& args);

//函数说明：获取下线玩家的cid		参数：无	返回值：cid值
void get_drop_game_player_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：设置game_player gate_cid  	参数：1.gate_cid 2.player_cid	返回值：无
void set_game_player_gate_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：game_server关闭客户端连接  	参数：1.gate_cid 2.player_cid 3,error_code	返回值：无
void game_close_client(const FunctionCallbackInfo<Value>& args);

//////////////////////////////////MasterServer相关函数////////////////////////////////
//函数说明：注册定时器到c++层		参数：1.定时器id 2.定时器间隔(毫秒单位) 3.从注册定时器到下次定时器到期中间间隔秒数	返回值：无
void register_master_timer(const FunctionCallbackInfo<Value>& args);
//函数说明：从c++层获取超时定时器	参数：无	返回值：到期的timer_id
void get_master_timer_id(const FunctionCallbackInfo<Value>& args);

//函数说明：获取gate消息object	参数：无	返回值：object对象
void pop_master_gate_msg_object(const FunctionCallbackInfo<Value>& args);
//函数说明：获取master消息object	参数：无	返回值：object对象
void pop_master_game_msg_object(const FunctionCallbackInfo<Value>& args);
//函数说明：获取db消息object	参数：无	返回值：object对象
void pop_master_db_msg_object(const FunctionCallbackInfo<Value>& args);
//函数说明：获取http消息object	参数：无	返回值：object对象
void pop_master_http_msg_object(const FunctionCallbackInfo<Value>& args);

//函数说明：发送消息object到gate_server	参数：1.gate_cid 2.player_cid 3.msg_id 4.status 5.消息object	返回值：无
void send_master_msg_to_gate(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到game_server	参数：1.game_cid 2.player_cid 3.msg_id 4.status 5.消息object	返回值：无
void send_master_msg_to_game(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到db_server	参数：1.msg_id 2.消息object返回值：无
void send_master_msg_to_db(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到log_server	参数：1.msg_id 2.消息object	返回值：无
void send_master_msg_to_log(const FunctionCallbackInfo<Value>& args);
//函数说明：发送消息object到http_server	参数：1.http_cid 2.msg_id 3.消息object	返回值：无
void send_master_msg_to_http(const FunctionCallbackInfo<Value>& args);

//函数说明：获取下线玩家的cid		参数：无	返回值：cid值
void get_drop_master_player_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：通过gate_cid获取master_player对象  	参数：1,gate_cid 2,player_cid	返回值：无
void set_master_player_gate_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：通过game_cid获取master_player对象  	参数：1,game_cid 2,player_cid	返回值：无
void set_master_player_game_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：master_server关闭客户端连接  	参数：1,gate_cid 2,player_cid 3,error_code	返回值：无
void master_close_client(const FunctionCallbackInfo<Value>& args);

void is_scene_in_process(const FunctionCallbackInfo<Value>& args);

void get_cid_function(const FunctionCallbackInfo<Value>& args);

void get_scene_entity_by_gate_cid(const FunctionCallbackInfo<Value>& args);
void get_scene_entity_npc(const FunctionCallbackInfo<Value>& args);
void thinking_in_ai(const FunctionCallbackInfo<Value>& args);

#endif /* SERVER_WRAP_H_ */
