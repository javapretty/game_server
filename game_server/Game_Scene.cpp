/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include "Log.h"
#include "Game_Scene.h"

Game_Scene::Game_Scene(int type, SCENE_ID id):
	scence_type_(type),
	scene_id_(id),
	aoi_manager_(new Aoi_Manager),
	map_(),
	grid_width_(0.f)
{
	load_scene_map();
}

Game_Scene::~Game_Scene(){

}

int Game_Scene::load_scene_map(){
//	map_ = {
//		{0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//		{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
//		{0,0,0,1,0,0,0,0,0,0,1,1,0,0,0},
//		{0,0,0,1,0,0,1,0,0,0,1,0,0,0,0},
//		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//	};
	return 0;
}

int Game_Scene::on_enter_scene(Scene_Entity *entity){
	entities_map_[entity->entity_id()] = entity;
	entity->scene(this);
	aoi_manager_->on_enter_aoi(entity->aoi_entity());
	return 0;
}

int Game_Scene::on_update_scene(Scene_Entity *entity) {
//	if(!check_move(entity->pos(), pos)){
//
//		return -1;
//	}
	//entity->on_update_position();
	aoi_manager_->on_update_aoi(entity->aoi_entity());
	return 0;
}

int Game_Scene::on_leave_scene(Scene_Entity *entity) {
	entities_map_.erase(entity->entity_id());
	entity->scene(NULL);
	aoi_manager_->on_leave_aoi(entity->aoi_entity());
	return 0;
}

bool Game_Scene::check_move(Position opos, Position pos) {
	return map_[(int)pos.x][(int)pos.y] != 1;
}
