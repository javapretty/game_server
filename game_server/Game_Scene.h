/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <boost/unordered_map.hpp>
#include "Scene_Entity.h"

typedef int32_t SCENE_ID;
typedef boost::unordered_map<ENTITY_ID, Scene_Entity *> ENTITIES_MAP;

class Game_Scene {
public:
	Game_Scene(int type, SCENE_ID id);
	~Game_Scene();
	int load_scene_map();
	int on_enter_scene(Scene_Entity *entity);
	int on_leave_scene(Scene_Entity *entity);

	inline SCENE_ID scene_id(){return scene_id_;}
private:
	int scence_type_;
	SCENE_ID scene_id_;
	ENTITIES_MAP entities_map_;
	int map_[15][15];
	float grid_width_;
};

#endif
