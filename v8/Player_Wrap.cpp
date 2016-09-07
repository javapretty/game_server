/*
 * Player_Wrap.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: zhangyalei
 */

#include "Player_Wrap.h"
#include "Game_Manager.h"
#include "Master_Manager.h"
#include "Scene_Manager.h"
#include "Msg_Manager.h"

extern Global<ObjectTemplate> _g_game_player_template;
extern Global<ObjectTemplate> _g_master_player_template;

Local<Object> wrap_game_player(Isolate* isolate, Game_Player *player) {
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> localTemplate = Local<ObjectTemplate>::New(isolate, _g_game_player_template);
	localTemplate->SetInternalFieldCount(1);
	Local<External> player_ptr = External::New(isolate, player);
	//将指针存在V8对象内部
	Local<Object> player_obj = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	player_obj->SetInternalField(0, player_ptr);

	return handle_scope.Escape(player_obj);
}

Game_Player *unwrap_game_player(Local<Object> obj) {
	Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<Game_Player*>(ptr);
}

void game_player_link_close(const FunctionCallbackInfo<Value>& args) {
	Game_Player *player = unwrap_game_player(args.Holder());
	if (!player) {
		return;
	}

	player->link_close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Local<Object> wrap_master_player(Isolate* isolate, Master_Player *player) {
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> localTemplate = Local<ObjectTemplate>::New(isolate, _g_master_player_template);
	localTemplate->SetInternalFieldCount(1);
	Local<External> player_ptr = External::New(isolate, player);
	//将指针存在V8对象内部
	Local<Object> player_obj = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	player_obj->SetInternalField(0, player_ptr);

	return handle_scope.Escape(player_obj);
}

Master_Player *unwrap_master_player(Local<Object> obj) {
	Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<Master_Player*>(ptr);
}

void master_player_link_close(const FunctionCallbackInfo<Value>& args) {
	Master_Player *player = unwrap_master_player(args.Holder());
	if (!player) {
		return;
	}

	player->link_close();
}
