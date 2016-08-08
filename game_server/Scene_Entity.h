/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#ifndef SCENE_ENTITY_H_
#define SCENE_ENTITY_H_

#include <string>
#include <stdint.h>
#include <math.h>
#include "Game_Struct.h"

typedef int64_t ENTITY_ID;

struct Position3D : public MSG {
	Position3D(float posx = 0, float posy = 0, float posz = 0);
	~Position3D();
	float operator - (Position3D sp);
	void setPosition(float posx = 0, float posy = 0, float posz = 0);
	void setPosition(Position3D pos);
	virtual void serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);
	virtual void reset(void);
	virtual void print(void);

	float x;
	float y;
	float z;
};

class Game_Scene;
class Scene_Entity;

typedef boost::unordered_map<ENTITY_ID, Scene_Entity *> AOI_MAP;

class Scene_Entity : public MSG {
public:
	Scene_Entity();
	~Scene_Entity();
	int on_update_position(Position3D new_pos);

	inline void set_scene(Game_Scene *scene){scene_ = scene;}
	inline ENTITY_ID entity_id(){return entity_id_;}
private:
	ENTITY_ID entity_id_;
	int64_t player_id_;
	Position3D pos_;
	float speedx_;
	float speedy_;
	float speedz_;
	Game_Scene *scene_;
	AOI_MAP aoi_map_;
	bool need_sync_;
};

#endif
