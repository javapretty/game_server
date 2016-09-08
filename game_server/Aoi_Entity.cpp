/*
 *  Created on: Aug 11, 2016
 *      Author: lijunliang
 */

#include "Log.h"
#include "Aoi_Entity.h"
#include "Scene_Entity.h"

Aoi_Entity::Aoi_Entity():
	entity_(),
	x_pos_(),
	y_pos_(),
	aoi_map_()
{

}

Aoi_Entity::~Aoi_Entity() {

}

void Aoi_Entity::add_aoi_entity(Aoi_Entity *entity, bool active) {
	if(aoi_map_.find(entity->entity_id()) != aoi_map_.end())
		return;
	aoi_map_[entity->entity_id()] = entity;
	if(active)
		entity->add_aoi_entity(this, false);
}

void Aoi_Entity::del_aoi_entity(Aoi_Entity *entity, bool active) {
	if(aoi_map_.find(entity->entity_id()) == aoi_map_.end())
		return;
	aoi_map_.erase(entity->entity_id());
	if(active)
		entity->del_aoi_entity(this, false);
}

void Aoi_Entity::update_aoi_map(AOI_MAP &new_map) {
	for(AOI_MAP::iterator iter = aoi_map_.begin();
			iter != aoi_map_.end(); iter++){
		if(new_map.find(iter->second->entity_id()) == new_map.end()){
			del_aoi_entity(iter->second, true);
		}
	}
	for(AOI_MAP::iterator iter = new_map.begin();
			iter != new_map.end(); iter++){
		if(aoi_map_.find(iter->second->entity_id()) == aoi_map_.end()){
			add_aoi_entity(iter->second, true);
		}
	}
//	LOG_ERROR("%d's aoi_map_size is %d", entity_id(), aoi_map_.size());
//	for(AOI_MAP::iterator iter = aoi_map_.begin();
//			iter != aoi_map_.end(); iter++){
//		LOG_ERROR("%d's aoi_entity is %d", entity_id(), iter->second->entity_id());
//	}
}

void Aoi_Entity::clear_aoi_map() {
	for(AOI_MAP::iterator iter = aoi_map_.begin(); iter != aoi_map_.end(); iter++){
		del_aoi_entity(iter->second, true);
	}
}

ENTITY_ID Aoi_Entity::entity_id(){
	return entity_->entity_id();
}

Position &Aoi_Entity::pos(){
	return entity_->pos();
}

Position &Aoi_Entity::opos(){
	return entity_->opos();
}

float Aoi_Entity::radius(){
	return entity_->radius();
}

void Aoi_Entity::reset() {
	entity_ = NULL;
	aoi_map_.clear();
}
