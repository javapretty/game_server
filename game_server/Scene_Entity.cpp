/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include "Scene_Entity.h"
#include "Scene_Manager.h"
#include "Game_Manager.h"
#include "Aoi_Entity.h"
#include "Server_Config.h"
#include "Aoi_Manager.h"

Scene_Entity::Scene_Entity():
	entity_id_(0),
	pos_(0, 0, 0),
	opos_(0, 0, 0),
	scene_(0),
	radius_(SERVER_CONFIG->server_misc()["aoi_radius"].asInt()),
	aoi_entity_(0)
{
}

Scene_Entity::~Scene_Entity() {

}

void Scene_Entity::reset() {
	entity_id_ = 0;
	pos_.reset();
	opos_.reset();
	scene_ =0;
	radius_ =0;
	if(aoi_entity_ != NULL) {
		Aoi_Manager::reclaim_aoi_entity(aoi_entity_);
		aoi_entity_= NULL;
	}
}

int Scene_Entity::on_update_position(Position new_pos) {
	opos_ = pos_;
	pos_ = new_pos;
	return 0;
}
