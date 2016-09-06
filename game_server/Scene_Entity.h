/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#ifndef SCENE_ENTITY_H_
#define SCENE_ENTITY_H_

#include <string>
#include <stdint.h>
#include <math.h>
#include <list>
#include "Public_Struct.h"
#include "Game_Player.h"

typedef int32_t ENTITY_ID;

class Aoi_Entity;
class Scene_Entity;
class Game_Scene;

typedef boost::unordered_map<ENTITY_ID, Scene_Entity *> ENTITIES_MAP;

class Scene_Entity {
public:
	Scene_Entity();
	~Scene_Entity();
	int on_update_position(Position new_pos);
	void reset();

	inline ENTITY_ID entity_id(){return entity_id_;}
	inline void set_entity_id(ENTITY_ID entity_id){entity_id_ = entity_id;}
	inline Position &pos(){return pos_;}
	inline Position &opos(){return opos_;}
	inline void scene(Game_Scene *scene){scene_ = scene;}
	inline Game_Scene *scene(){return scene_;}
	inline float radius(){return radius_;}
	inline Aoi_Entity *aoi_entity(){return aoi_entity_;}
	inline void set_aoi_entity(Aoi_Entity *entity){aoi_entity_ = entity;}
private:
	ENTITY_ID entity_id_;
	Position pos_, opos_;
	Game_Scene *scene_;
	float radius_;
	Aoi_Entity *aoi_entity_;
};

#endif
