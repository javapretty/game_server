/*
 * Player_Wrap.cpp
 *
 *  Created on: 2016年2月20日
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

void enter_scene(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 4) {
		LOG_ERROR("enter_scene args error, length: %d\n", args.Length());
		return;
	}
	Game_Player *player = unwrap_game_player(args.Holder());
	if(!player) {
		return;
	}
	int scene_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int x = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int y = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int z = args[3]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	player->scene_entity()->pos().set_position(x, y, z);
	Game_Scene *scene = SCENE_MANAGER->get_scene(scene_id);
	if(scene == NULL) {
		LOG_ERROR("Scene %d not exist!", scene_id);
		return;
	}
	scene->on_enter_scene(player->scene_entity());
}

void leave_scene(const FunctionCallbackInfo<Value>& args) {
	Game_Player *player = unwrap_game_player(args.Holder());
	if(!player) {
		return;
	}
	Game_Scene *scene = player->scene_entity()->scene();
	if(scene != NULL){
			scene->on_leave_scene(player->scene_entity());
	}
}

void move_to_point(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("move_to_point args error, length: %d\n", args.Length());
		return;
	}
	Game_Player *player = unwrap_game_player(args.Holder());
	if(!player) {
		return;
	}

	int x = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int y = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int z = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);

	Game_Scene *scene = player->scene_entity()->scene();
	if(scene != NULL){
		scene->on_move_scene(player->scene_entity(), Position(x,y,z));
	}
}

void set_aoi_info(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 1) {
		LOG_ERROR("set_aoi_info args error, length: %d\n", args.Length());
		return;
	}
	Game_Player *player = unwrap_game_player(args.Holder());
	if(!player) {
		return;
	}
	Struct_Name_Map::iterator iter = MSG_MANAGER->msg_struct_name_map().find("Aoi_Info");
	if(iter != MSG_MANAGER->msg_struct_name_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		GUARD(Thread_Mutex, mon, SCENE_MANAGER->lock());
		player->scene_entity()->aoi_info().reset();
		msg_struct->build_msg_buffer(args.GetIsolate(), args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), player->scene_entity()->aoi_info());
	}
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
