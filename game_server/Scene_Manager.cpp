/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include <Scene_Manager.h>

Scene_Manager *Scene_Manager::instance_ = NULL;

Scene_Manager *Scene_Manager::instance() {
	if(instance_ == NULL)
		instance_ = new Scene_Manager();
	return instance_;
}

Scene_Manager::Scene_Manager():
		scenes_map_(),
		auto_allocated_id_(0)
{
}

Scene_Manager::~Scene_Manager(){

}

int Scene_Manager::create_new_scene(){
	Game_Scene *scene = new Game_Scene(1001, auto_allocated_id_);
	scenes_map_[auto_allocated_id_++] = scene;
	return 0;
}

Game_Scene *Scene_Manager::get_scene(SCENE_ID scene_id){
	SCENES_MAP::iterator iter = scenes_map_.find(scene_id);
	if(iter != scenes_map_.end())
		return iter->second;
	return NULL;
}
