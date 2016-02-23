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

void process_login_buffer(const FunctionCallbackInfo<Value>& args);
void get_player_data(const FunctionCallbackInfo<Value>& args);
void get_player(const FunctionCallbackInfo<Value>& args);
void respond_success_result(const FunctionCallbackInfo<Value>& args);
void respond_error_result(const FunctionCallbackInfo<Value>& args);
void role_id(const FunctionCallbackInfo<Value>& args);
void role_name(const FunctionCallbackInfo<Value>& args);

#endif /* PLAYER_WRAP_H_ */
