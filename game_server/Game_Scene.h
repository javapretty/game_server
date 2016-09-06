/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "Scene_Entity.h"
#include "Aoi_Manager.h"

typedef int32_t SCENE_ID;

class Game_Scene {
public:
	Game_Scene(int type, SCENE_ID id);
	~Game_Scene();
	int load_scene_map();
	int on_enter_scene(Scene_Entity *entity);
	int on_update_scene(Scene_Entity *entity);
	int on_leave_scene(Scene_Entity *entity);

	inline SCENE_ID scene_id(){return scene_id_;}
private:
	bool check_move(Position opos, Position pos);
private:
	int scence_type_;
	SCENE_ID scene_id_;
	ENTITIES_MAP entities_map_;
	Aoi_Manager *aoi_manager_;
	int map_[15][15];
	float grid_width_;
};

#endif
