/*
 *  Created on: Aug 11, 2016
 *      Author: lijunliang
 */

#include "Aoi_Entity.h"
#include "Scene_Entity.h"

Aoi_Entity::Aoi_Entity(Scene_Entity *entity):
	entity_(entity),
	x_pos_(),
	y_pos_(),
	aoi_map_()
{

}

Aoi_Entity::~Aoi_Entity() {

}

void Aoi_Entity::add_aoi_entity(Aoi_Entity *entity) {
	if(aoi_map_.find(entity->entity_id()) != aoi_map_.end())
		return;
	aoi_map_[entity->entity_id()] = entity;
	set_sync();
	entity->add_aoi_entity(this);
}

void Aoi_Entity::del_aoi_entity(Aoi_Entity *entity) {
	if(aoi_map_.find(entity->entity_id()) == aoi_map_.end())
		return;
	aoi_map_.erase(entity->entity_id());
	set_sync();
	entity->del_aoi_entity(this);
}

void Aoi_Entity::update_aoi_map(AOI_MAP &new_map) {
	for(AOI_MAP::iterator iter = aoi_map_.begin();
			iter != aoi_map_.end(); iter++){
		if(new_map.find(iter->second->entity_id()) == new_map.end()){
			del_aoi_entity(iter->second);
		}
	}
	for(AOI_MAP::iterator iter = new_map.begin();
			iter != new_map.end(); iter++){
		if(aoi_map_.find(iter->second->entity_id()) == aoi_map_.end()){
			add_aoi_entity(iter->second);
		}
	}
}

void Aoi_Entity::clear_aoi_map() {
	for(AOI_MAP::iterator iter = aoi_map_.begin(); iter != aoi_map_.end(); iter++){
		del_aoi_entity(iter->second);
	}
}

void Aoi_Entity::broadcast_sync() {
	for(AOI_MAP::iterator iter = aoi_map_.begin(); iter != aoi_map_.end(); iter++){
		iter->second->set_sync();
	}
}

void Aoi_Entity::broadcast_aoi_info() {
	Block_Buffer buf;
	buf.write_uint16(aoi_map_.size());
	for(AOI_MAP::iterator iter = aoi_map_.begin(); iter != aoi_map_.end(); iter++){
		Aoi_Entity *entity = iter->second;
		entity->scene_entity()->write_aoi_info(buf);
	}
	entity_->game_player()->respond_success_result(520400, &buf);
	set_sync(false);
}
