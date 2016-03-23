/*
 * Player_Wrap.h
 *
 *  Created on: 2016年2月20日
 *      Author: zhangyalei
 */

#ifndef PLAYER_WRAP_H_
#define PLAYER_WRAP_H_

#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "Game_Player.h"

using namespace v8;

Local<Object> wrap_player(Isolate* isolate, Game_Player *player);
Game_Player *unwrap_player(Local<Object> obj);

//函数说明：通过cid获取player对象  	参数：1,gate_cid 2,player_cid	返回值：player对象
void get_player_by_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：通过role_name获取player对象  	参数：1,role_name	返回值：player对象
void get_player_by_name(const FunctionCallbackInfo<Value>& args);
//函数说明：获取player_data buffer对象 	参数：无	返回值：buffer对象
void get_player_data_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：发送成功的消息buffer到client 	参数：1,msg_id 2,buffer对象	返回值：无
void respond_success_result(const FunctionCallbackInfo<Value>& args);
//函数说明：发送错误消息号到client 	参数：1,msg_id 2,错误号	返回值：无
void respond_error_result(const FunctionCallbackInfo<Value>& args);

#endif /* PLAYER_WRAP_H_ */
