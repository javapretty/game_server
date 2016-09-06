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

//函数说明：game_player关闭连接 	参数：无	返回值：无
void game_player_link_close(const FunctionCallbackInfo<Value>& args);

/////////////////////////////Master_Player相关函数////////////////////////////////////////
Local<Object> wrap_master_player(Isolate* isolate, Master_Player *player);
Master_Player *unwrap_master_player(Local<Object> obj);

//函数说明：master_player关闭连接 	参数：无	返回值：无
void master_player_link_close(const FunctionCallbackInfo<Value>& args);

#endif /* PLAYER_WRAP_H_ */
