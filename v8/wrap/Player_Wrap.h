/*
 * Player_Wrap.h
 *
 *  Created on: 2016年2月20日
 *      Author: zhangyalei
 */

#ifndef PLAYER_WRAP_H_
#define PLAYER_WRAP_H_

#include "include/v8.h"
#include "Game_Player.h"
#include "Master_Player.h"

using namespace v8;

//////////////////////////////Game_Player相关函数////////////////////////////////////////////////
Local<Object> wrap_game_player(Isolate* isolate, Game_Player *player);
Game_Player *unwrap_game_player(Local<Object> obj);

//函数说明：获取game_player save_data buffer对象 	参数：无	返回值：buffer对象
void get_game_player_save_data_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：发送成功的消息buffer到client 	参数：1,msg_id 2,buffer对象	返回值：无
void game_player_respond_success_result(const FunctionCallbackInfo<Value>& args);
//函数说明：发送错误消息号到client 	参数：1,msg_id 2,错误号	返回值：无
void game_player_respond_error_result(const FunctionCallbackInfo<Value>& args);

/////////////////////////////Master_Player相关函数////////////////////////////////////////
Local<Object> wrap_master_player(Isolate* isolate, Master_Player *player);
Master_Player *unwrap_master_player(Local<Object> obj);

//函数说明：获取master_player save_data buffer对象 	参数：无	返回值：buffer对象
void get_master_player_save_data_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：发送成功的消息buffer到client 	参数：1,msg_id 2,buffer对象	返回值：无
void master_player_respond_success_result(const FunctionCallbackInfo<Value>& args);
//函数说明：发送错误消息号到client 	参数：1,msg_id 2,错误号	返回值：无
void master_player_respond_error_result(const FunctionCallbackInfo<Value>& args);

#endif /* PLAYER_WRAP_H_ */
