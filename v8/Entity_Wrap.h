/*
 * Entity_Wrap.h
 *
 *  Created on: Sep 3, 2016
 *      Author: lijunliang
 */

#ifndef ENTITY_WRAP_H_
#define ENTITY_WRAP_H_

#include "include/v8.h"
#include "Scene_Entity.h"

using namespace v8;

Local<Object> wrap_scene_entity(Isolate* isolate, Scene_Entity *entity);
Scene_Entity *unwrap_scene_entity(Local<Object> obj);

void enter_scene(const FunctionCallbackInfo<Value>& args);
void update_position(const FunctionCallbackInfo<Value>& args);
void leave_scene(const FunctionCallbackInfo<Value>& args);
void get_aoi_list(const FunctionCallbackInfo<Value>& args);
void broadcast_msg_to_all(const FunctionCallbackInfo<Value>& args);
void broadcast_msg_to_all_without_self(const FunctionCallbackInfo<Value>& args);
void reclaim_scene_entity(const FunctionCallbackInfo<Value>& args);

void get_entity_x(Local<String> property, const PropertyCallbackInfo<Value>& args);
void set_entity_x(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args);
void get_entity_y(Local<String> property, const PropertyCallbackInfo<Value>& args);
void set_entity_y(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args);
void get_entity_z(Local<String> property, const PropertyCallbackInfo<Value>& args);
void set_entity_z(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args);
void get_scene_id(Local<String> property, const PropertyCallbackInfo<Value>& args);
void set_scene_id(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& args);

#endif /* ENTITY_WRAP_H_ */
