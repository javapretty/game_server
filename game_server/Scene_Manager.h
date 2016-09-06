/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "Game_Scene.h"
#include "Public_Struct.h"
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"

typedef boost::unordered_map<SCENE_ID, Game_Scene *> SCENES_MAP;
typedef Object_Pool<Scene_Entity, Spin_Lock> Scene_Entity_Pool;

class Scene_Manager {
private:
	Scene_Manager();
	~Scene_Manager();
public:
	static Scene_Manager *instance();
	int load_scene();
	int create_new_scene(int type);
	Scene_Entity *create_scene_entity(ENTITY_ID entity_id);
	void reclaim_scene_entity(Scene_Entity *entity);
	Game_Scene *get_scene(SCENE_ID scene_id);
	void tick(Time_Value now);
	bool has_scene(int scene_id);
private:
	int create_scene_id(int type);
private:
	static Scene_Manager *instance_;
	Scene_Entity_Pool scene_entity_pool_;
	SCENES_MAP scenes_map_;
};

#define SCENE_MANAGER Scene_Manager::instance()

#endif
