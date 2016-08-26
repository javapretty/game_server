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

typedef int64_t ENTITY_ID;

class Aoi_Entity;
class Scene_Entity;
class Game_Scene;

typedef boost::unordered_map<ENTITY_ID, Scene_Entity *> ENTITIES_MAP;

class Scene_Entity {
public:
	Scene_Entity();
	~Scene_Entity();
	int on_update_position(Position3D new_pos);
	void write_aoi_info(Block_Buffer &buffer);
	void broadcast_aoi_info();
	void reset();

	inline ENTITY_ID entity_id(){return entity_id_;}
	inline void set_entity_id(ENTITY_ID id){entity_id_ = id;}
	inline Position3D pos(){return pos_;}
	inline Position3D opos(){return opos_;}
	inline void scene(Game_Scene *scene){scene_ = scene;}
	inline Game_Scene *scene(){return scene_;}
	inline float radius(){return radius_;}
	inline Aoi_Entity *aoi_entity(){return aoi_entity_;}
	inline void set_aoi_entity(Aoi_Entity *entity){aoi_entity_ = entity;}
	inline Block_Buffer &aoi_info(){return extra_info_;}
	inline bool need_sync(){return need_sync_;}
	inline void set_sync(bool flag){need_sync_ = flag;}
	inline Game_Player *game_player(){return player_;}
	inline void set_game_player(Game_Player *game_player){player_ = game_player;}
private:
	ENTITY_ID entity_id_;
	Game_Player *player_;
	Position3D pos_, opos_;
	Game_Scene *scene_;
	float radius_;
	Aoi_Entity *aoi_entity_;
	bool need_sync_;
	Block_Buffer extra_info_;
};

#endif
