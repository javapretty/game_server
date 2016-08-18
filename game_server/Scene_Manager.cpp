/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include "Scene_Manager.h"
#include "Server_Config.h"
#include "Game_Manager.h"

Scene_Manager *Scene_Manager::instance_ = NULL;

Scene_Manager *Scene_Manager::instance() {
	if(instance_ == NULL)
		instance_ = new Scene_Manager();
	return instance_;
}

Scene_Manager::Scene_Manager():
		scenes_map_(),
		auto_allocated_id_(0),
		aoi_broadcast_interval_(SERVER_CONFIG->server_misc()["aoi_broadcast_interval"].asInt()),
		last_sync_(0)
{

}

Scene_Manager::~Scene_Manager(){

}

int Scene_Manager::load_scene() {
	create_new_scene(1001);
	return 0;
}

int Scene_Manager::create_new_scene(int type){
	Game_Scene *scene = new Game_Scene(type, create_scene_id(type));
	scenes_map_[scene->scene_id()] = scene;
	Block_Buffer buffer;
	buffer.make_player_message(160102, 0, 0);
	buffer.write_int32(scene->scene_id());
	buffer.write_int32(GAME_MANAGER->server_id());
	buffer.finish_message();
	GAME_MANAGER->send_to_master(buffer);
	return 0;
}

Game_Scene *Scene_Manager::get_scene(SCENE_ID scene_id){
	SCENES_MAP::iterator iter = scenes_map_.find(scene_id);
	if(iter != scenes_map_.end())
		return iter->second;
	return NULL;
}

void Scene_Manager::tick(Time_Value now) {
	if(now - last_sync_ >= Time_Value(0, 1000 * aoi_broadcast_interval_)){
		GUARD(Thread_Mutex, mon, lock_);
		for(SCENES_MAP::iterator iter = scenes_map_.begin(); iter != scenes_map_.end(); iter++){
			iter->second->broadcast_aoi_info();
		}
		last_sync_ = now;
	}
}

int Scene_Manager::create_scene_id(int type) {
	return GAME_MANAGER->server_id() * 10000 + type;
}

Thread_Mutex &Scene_Manager::lock() {
	return lock_;
}

bool Scene_Manager::has_scene(int scene_id) {
	SCENES_MAP::iterator iter = scenes_map_.find(scene_id);
	return iter != scenes_map_.end();
}
