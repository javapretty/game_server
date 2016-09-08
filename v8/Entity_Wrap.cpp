/*
 * Entity_Wrap.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: lijunliang
 */

#include "Entity_Wrap.h"
#include "Game_Manager.h"
#include "Master_Manager.h"
#include "Scene_Manager.h"
#include "Msg_Manager.h"

extern Global<ObjectTemplate> _g_scene_entity_template;

Local<Object> wrap_scene_entity(Isolate* isolate, Scene_Entity *entity) {
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> localTemplate = Local<ObjectTemplate>::New(isolate, _g_scene_entity_template);
	localTemplate->SetInternalFieldCount(1);
	Local<External> entity_ptr = External::New(isolate, entity);
	//将指针存在V8对象内部
	Local<Object> entity_obj = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	entity_obj->SetInternalField(0, entity_ptr);

	return handle_scope.Escape(entity_obj);
}

Scene_Entity *unwrap_scene_entity(Local<Object> obj) {
	Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<Scene_Entity*>(ptr);
}

void enter_scene(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 4) {
		LOG_ERROR("enter_scene args error, length: %d\n", args.Length());
		return;
	}
	Scene_Entity *entity = unwrap_scene_entity(args.Holder());
	if(!entity) {
		return;
	}
	int scene_id = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int x = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int y = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	int z = args[3]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	entity->pos().set_position(x, y, z);
	Game_Scene *scene = SCENE_MANAGER->get_scene(scene_id);
	if(scene == NULL) {
		LOG_ERROR("scene %d not exist!", scene_id);
		return;
	}
	scene->on_enter_scene(entity);
}

void leave_scene(const FunctionCallbackInfo<Value>& args) {
	Scene_Entity *entity = unwrap_scene_entity(args.Holder());
	if(!entity) {
		return;
	}
	Game_Scene *scene = entity->scene();
	if(scene != NULL){
			scene->on_leave_scene(entity);
	}
}

void update_position(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 0) {
		LOG_ERROR("move_to_point args error, length: %d\n", args.Length());
		return;
	}
	Scene_Entity *entity = unwrap_scene_entity(args.Holder());
	if(!entity) {
		return;
	}

//	int x = args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
//	int y = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
//	int z = args[2]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);

	Game_Scene *scene = entity->scene();
	if(scene != NULL){
		scene->on_update_scene(entity);
	}
}

void get_aoi_list(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 0) {
		LOG_ERROR("get_aoi_list args error, length: %d\n", args.Length());
		return;
	}
	Scene_Entity *entity = unwrap_scene_entity(args.Holder());
	if(!entity) {
		return;
	}
	AOI_MAP aoi_map = entity->aoi_entity()->aoi_map();
	Isolate *isolate = args.GetIsolate();
	EscapableHandleScope handle_scope(isolate);

	Local<Array> list_obj = Array::New(isolate, aoi_map.size());
	int index = 0;
	for(AOI_MAP::iterator iter = aoi_map.begin(); iter != aoi_map.end(); iter++) {
		if(iter->second->entity_id() == 0)
			continue;
		Local<Value> value = Number::New(isolate, iter->second->entity_id());
		list_obj->Set(isolate->GetCurrentContext(), index, value).FromJust();
		index++;
	}

	args.GetReturnValue().Set(handle_scope.Escape(list_obj));
}

void get_entity_x(Local<String> property, const PropertyCallbackInfo<Value>& args){
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	int x = entity->pos().x;
	args.GetReturnValue().Set(x);
}

void set_entity_x(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args){
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	entity->pos().x = value->Int32Value();
}

void get_entity_y(Local<String> property, const PropertyCallbackInfo<Value>& args){
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	int y = entity->pos().y;
	args.GetReturnValue().Set(y);
}

void set_entity_y(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args){
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	entity->pos().y = value->Int32Value();
}

void get_entity_z(Local<String> property, const PropertyCallbackInfo<Value>& args){
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	int z = entity->pos().z;
	args.GetReturnValue().Set(z);
}

void set_entity_z(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args){
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	entity->pos().z = value->Int32Value();
}

void broadcast_msg_to_all_without_self(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("broadcast_msg_to_all args error, length: %d\n", args.Length());
		return;
	}
	Isolate *isolate = args.GetIsolate();
	Local<Object> list_obj = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
	int msg_id = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	if (!list_obj->IsArray()) {
		LOG_ERROR("broadcast_msg_to_all arg[0] is not array");
		return;
	}

	Block_Buffer msg_buf;
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[2]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), msg_buf);
	}

	Local<Array> array = Local<Array>::Cast(list_obj);
	int16_t len = array->Length();
	for (int i = 0; i < len; ++i) {
		Local<Value> cid_obj = array->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
		int32_t cid = cid_obj->IntegerValue(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		if(cid == 0){
			i++;
			continue;
		}
		int gate_cid = GET_GATE_CID(cid);
		int player_cid = GET_PLAYER_CID(cid);
		int status = 0;
		Block_Buffer buf;
		buf.make_player_message(msg_id, status, player_cid);
		buf.copy(&msg_buf);
		buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, buf);
	}
}

void broadcast_msg_to_all(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() != 3) {
		LOG_ERROR("broadcast_msg_to_all args error, length: %d\n", args.Length());
		return;
	}
	Isolate *isolate = args.GetIsolate();
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	Local<Object> list_obj = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
	int msg_id = args[1]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
	if (!list_obj->IsArray()) {
		LOG_ERROR("broadcast_msg_to_all arg[0] is not array");
		return;
	}

	Block_Buffer msg_buf;
	Struct_Id_Map::iterator iter = MSG_MANAGER->msg_struct_id_map().find(msg_id);
	if(iter != MSG_MANAGER->msg_struct_id_map().end()){
		Msg_Struct *msg_struct  = dynamic_cast<Msg_Struct*>(iter->second);
		msg_struct->build_msg_buffer(args.GetIsolate(), args[2]->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked(), msg_buf);
	}

	Local<Array> array = Local<Array>::Cast(list_obj);
	int16_t len = array->Length();
	for (int i = 0; i < len + 1; ++i) {
		int32_t cid = 0;
		if(i == len){
			cid = entity->entity_id();
		}
		else {
			Local<Value> cid_obj = array->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
			cid = cid_obj->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
		}
		if(cid == 0){
			i++;
			continue;
		}
		int gate_cid = GET_GATE_CID(cid);
		int player_cid = GET_PLAYER_CID(cid);
		int status = 0;
		Block_Buffer buf;
		buf.make_player_message(msg_id, status, player_cid);
		buf.copy(&msg_buf);
		buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, buf);
	}
}

void reclaim_scene_entity(const FunctionCallbackInfo<Value>& args) {
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	SCENE_MANAGER->reclaim_scene_entity(entity);
}

void get_scene_id(Local<String> property, const PropertyCallbackInfo<Value>& args) {
	Local<Object> self = args.Holder();
	Scene_Entity* entity = unwrap_scene_entity(self);
	int scene_id = entity->scene()->scene_id();
	args.GetReturnValue().Set(scene_id);
}

void set_scene_id(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args) {
	LOG_ERROR("Can't set entity's scene_id!");
}
