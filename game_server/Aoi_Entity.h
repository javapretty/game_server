/*
 *  Created on: Aug 11, 2016
 *      Author: lijunliang
 */

#ifndef AOI_ENTITY_H_
#define AOI_ENTITY_H_

#include <string>
#include <stdint.h>
#include <math.h>
#include <list>
#include "Public_Struct.h"
#include "Scene_Entity.h"

class Aoi_Entity;

typedef std::list<Aoi_Entity *> AOI_LIST;
typedef boost::unordered_map<ENTITY_ID, Aoi_Entity *> AOI_MAP;

class Aoi_Entity {
public:
	Aoi_Entity();
	~Aoi_Entity();
	void update_aoi_map(AOI_MAP &new_map);
	void add_aoi_entity(Aoi_Entity *entity, bool active);
	void del_aoi_entity(Aoi_Entity *entity, bool active);
	void clear_aoi_map();
	ENTITY_ID entity_id();
	Position &pos();
	Position &opos();
	float radius();
	void reset();

	inline Scene_Entity *scene_entity(){return entity_;}
	inline void set_scene_entity(Scene_Entity *entity){entity_ = entity;}
	inline void x_pos(AOI_LIST::iterator iter){x_pos_ = iter;}
	inline void y_pos(AOI_LIST::iterator iter){y_pos_ = iter;}
	inline AOI_LIST::iterator x_pos(){return x_pos_;}
	inline AOI_LIST::iterator y_pos(){return y_pos_;}
	inline AOI_MAP &aoi_map(){return aoi_map_;}
private:
	Scene_Entity *entity_;
	AOI_LIST::iterator x_pos_;
	AOI_LIST::iterator y_pos_;
	AOI_MAP aoi_map_;
};

#endif
