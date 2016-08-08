/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include <Game_Scene.h>

Game_Scene::Game_Scene(int type, SCENE_ID id):
	scence_type_(type),
	scene_id_(id),
	entities_map_(),
	map_()
{

}

Game_Scene::~Game_Scene(){

}

int Game_Scene::load_scene_map(){
	map_ = {
		{0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,1,1,0,0,0},
		{0,0,0,1,0,0,1,0,0,0,1,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	return 0;
}

int Game_Scene::on_enter_scene(Scene_Entity *entity){
	entities_map_[entity->entity_id()] = entity;
	return 0;
}

int Game_Scene::on_leave_scene(Scene_Entity *entity){
	return 0;
}
