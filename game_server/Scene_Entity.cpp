/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include "Scene_Entity.h"
#include "Scene_Manager.h"
#include "Game_Manager.h"
#include "Aoi_Entity.h"

Scene_Entity::Scene_Entity(Game_Player *game_player):
	entity_id_(1),
	player_(game_player),
	pos_(0, 0, 0),
	opos_(0, 0, 0),
	scene_(0),
	radius_(3),
	aoi_entity_(new Aoi_Entity(this)),
	need_sync_(false),
	extra_info_()
{
	entity_id_ = SCENE_MANAGER->get_next_id();
}

Scene_Entity::~Scene_Entity() {

}

int Scene_Entity::on_update_position(Position3D new_pos) {
	opos_ = pos_;
	pos_ = new_pos;
	return 0;
}

void Scene_Entity::write_aoi_info(Block_Buffer &buffer) {
	buffer.write_double(pos_.x);
	buffer.write_double(pos_.y);
	buffer.write_double(pos_.z);
	buffer.copy(&extra_info_);
}

void Scene_Entity::broadcast_aoi_info() {
	aoi_entity_->broadcast_aoi_info();
}
