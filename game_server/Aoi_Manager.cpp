/*
 *  Created on: Aug 10, 2016
 *      Author: lijunliang
 */

#include "Log.h"
#include "Aoi_Manager.h"

Aoi_Manager::Aoi_Manager():
	x_list_(),
	y_list_()
{

}

Aoi_Manager::~Aoi_Manager(){

}

Aoi_Entity_Pool Aoi_Manager::aoi_entity_pool_;

Aoi_Entity *Aoi_Manager::create_aoi_entity(Scene_Entity *entity) {
	Aoi_Entity *aoi_entity = aoi_entity_pool_.pop();
	aoi_entity->set_scene_entity(entity);
	return aoi_entity;
}

void Aoi_Manager::reclaim_aoi_entity(Aoi_Entity *entity) {
	entity->reset();
	aoi_entity_pool_.push(entity);
}

int Aoi_Manager::on_enter_aoi(Aoi_Entity *entity){
	insert_entity(entity);
	update_aoi_map(entity);
	return 0;
}

int Aoi_Manager::on_update_aoi(Aoi_Entity *entity) {
	bool x_direct = (entity->pos().x - entity->opos().x > 0 ? true : false);
	bool y_direct = (entity->pos().y - entity->opos().y > 0 ? true : false);
	update_list(entity, x_direct, 0);
	update_list(entity, y_direct, 1);
	update_aoi_map(entity);
	return 0;
}

int Aoi_Manager::on_leave_aoi(Aoi_Entity *entity){
	x_list_.erase(entity->x_pos());
	y_list_.erase(entity->y_pos());
	entity->clear_aoi_map();
	return 0;
}

void Aoi_Manager::insert_entity(Aoi_Entity *entity) {
	AOI_LIST::iterator iter;
	AOI_LIST::iterator pos = x_list_.end();
	for(iter = x_list_.begin(); iter != x_list_.end(); iter++){
		float diff = (*iter)->pos().x - entity->pos().x;
		if(diff > 0){
			pos = iter;
			break;
		}
	}
	x_list_.insert(pos, entity);
	entity->x_pos(--pos);

	pos = y_list_.end();
	for(iter = y_list_.begin(); iter != y_list_.end(); iter++){
		float diff = (*iter)->pos().y - entity->pos().y;
		if(diff > 0){
			pos = iter;
			break;
		}
	}
	y_list_.insert(pos, entity);
	entity->y_pos(--pos);
}

void Aoi_Manager::update_list(Aoi_Entity *entity, bool direct, int xy) {
	AOI_LIST::iterator iter, pos;
	if(xy == 0){
		iter = entity->x_pos();
		if(direct) {
			if(iter != x_list_.begin()){
				iter = --entity->x_pos();
				x_list_.erase(entity->x_pos());
				while(1){
					if((*iter)->pos().x < entity->pos().x){
						pos = ++iter;
						break;
					}
					if(iter == x_list_.begin()){
						pos = x_list_.begin();
						break;
					}
					iter--;
				}
				x_list_.insert(pos, entity);
				entity->x_pos(--pos);
			}
		}
		else {
			if(iter != --x_list_.end()){
				iter = ++entity->x_pos();
				x_list_.erase(entity->x_pos());
				while(1){
					if((*iter)->pos().x > entity->pos().x){
						pos = iter;
						break;
					}
					if(iter == --x_list_.end()){
						pos = x_list_.end();
						break;
					}
					iter++;
				}
				x_list_.insert(pos, entity);
				entity->x_pos(--pos);
			}
		}
	}
	else if(xy == 1){
		iter = entity->y_pos();
		if(direct) {
			if(iter != y_list_.begin()){
				iter = --entity->y_pos();
				y_list_.erase(entity->y_pos());
				while(1){
					if((*iter)->pos().y < entity->pos().y){
						pos = ++iter;
						break;
					}
					if(iter == y_list_.begin()){
						pos = y_list_.begin();
						break;
					}
					iter--;
				}
				y_list_.insert(pos, entity);
				entity->y_pos(--pos);
			}
		}
		else {
			if(iter != --y_list_.end()){
				iter = ++entity->y_pos();
				y_list_.erase(entity->y_pos());
				while(1){
					if((*iter)->pos().y > entity->pos().y){
						pos = iter;
						break;
					}
					if(iter == --y_list_.end()){
						pos = y_list_.end();
						break;
					}
					iter++;
				}
				y_list_.insert(pos, entity);
				entity->y_pos(--pos);
			}
		}
	}
}

void Aoi_Manager::update_aoi_map(Aoi_Entity *entity) {
	AOI_MAP x_map, new_map;
	AOI_LIST::iterator iter;
	iter = entity->x_pos();
	while(1){
		iter++;
		if(iter == x_list_.end())
			break;
		if((*iter)->pos().x - entity->pos().x > entity->radius())
			break;
		x_map[(*iter)->entity_id()] = *iter;
	}
	iter = entity->x_pos();
	while(1){
		if(iter == x_list_.begin())
			break;
		iter--;
		if(entity->pos().x - (*iter)->pos().x > entity->radius())
			break;
		x_map[(*iter)->entity_id()] = *iter;
	}

	iter = entity->y_pos();
	while(1){
		iter++;
		if(iter == y_list_.end())
			break;
		if((*iter)->pos().y - entity->pos().y > entity->radius())
			break;
		if(x_map.find((*iter)->entity_id()) != x_map.end())
			new_map[(*iter)->entity_id()] = *iter;
	}
	iter = entity->y_pos();
	while(1){
		if(iter == y_list_.begin())
			break;
		iter--;
		if(entity->pos().y - (*iter)->pos().y > entity->radius())
			break;
		if(x_map.find((*iter)->entity_id()) != x_map.end())
			new_map[(*iter)->entity_id()] = *iter;
	}

	entity->update_aoi_map(new_map);
}
